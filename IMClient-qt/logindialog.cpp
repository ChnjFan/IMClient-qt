#include "logindialog.h"

#include <QJsonObject>
#include <QUrl>
#include <QString>
#include <QCryptographicHash>
#include <QWidgetAction>

#include "ui_logindialog.h"
#include "httpmgr.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 连接按钮事件
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);
    // 登录结果事件
    initHttpHandlers();
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_login_mod_finish,
            this, &LoginDialog::slot_login_mod_finish);

    // 忘记密码设置状态
    ui->forget_label->setState("normal", "hover", "", "select", "select_hover", "");
    connect(ui->forget_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_passwd);
    ui->passwd_visible->setState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
    ui->passwd_edit->setEchoMode(QLineEdit::Password);
    connect(ui->passwd_visible, &ClickedLabel::clicked, this, [this](){
        auto state = ui->passwd_visible->getCurState();
        if (state == ClickLbState::Normal) {
            ui->passwd_edit->setEchoMode(QLineEdit::Password);
        }
        else {
            ui->passwd_edit->setEchoMode(QLineEdit::Normal);
        }
    });

    // 提示
    ui->err_tip->clear();
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::initHttpHandlers()
{
    handlers_.insert(ReqId::ID_USER_LOGIN, [this](const QJsonObject& jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("用户登录失败"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        auto uid = jsonObj["uid"].toInt();

        // todo: 根据服务器返回聊天服务地址，建立 TCP 长连接
        emit sig_connect_tcp();

        showTip(tr("登录成功"), true);
        qDebug() << "uid is " << uid;
    });
}

void LoginDialog::AddTipErr(TipErr te, QString tips)
{
    tip_error_[te] = tips;
    showTip(tips, false);
}

void LoginDialog::DelTipErr(TipErr te)
{
    tip_error_.remove(te);
    if (tip_error_.empty()) {
        ui->err_tip->clear(); // 所有错误提示完后清空界面
        return;
    }
    showTip(tip_error_.first(), false);
}

void LoginDialog::showTip(QString str, bool is_ok)
{
    ui->err_tip->setText(str);
    if (is_ok) {
        ui->err_tip->setProperty("state", "normal");
    }
    else {
        ui->err_tip->setProperty("state", "err");
    }

    repolish(ui->err_tip);
}


bool LoginDialog::checkUserValid()
{
    auto email = ui->email_edit->text();
    if (email.isEmpty()) {
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("请输入邮箱"));
        return false;
    }

    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    if (!regex.match(email).hasMatch()) {
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱输入错误"));
        return false;
    }

    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool LoginDialog::checkPasswdValid()
{
    auto passwd = ui->passwd_edit->text();

    if (passwd.length() < 6 || passwd.length() > 15) {
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应该在 6～15"));
        return false;
    }

    QRegularExpression regex(R"(^[a-zA-Z0-9!@#$%^&*]{6,15}$)");
    if (!regex.match(passwd).hasMatch()) {
        AddTipErr(TipErr::TIP_PWD_ERR, tr("密码不能包含特殊字符"));
        return false;
    }
    DelTipErr(TipErr::TIP_PWD_ERR);
    return true;
}

void LoginDialog::slot_forget_passwd()
{
    qDebug() << "Switch reset passwd";
    emit switchReset();
}

void LoginDialog::on_login_btn_clicked()
{
    if (!checkUserValid() || !checkPasswdValid()) {
        return;
    }

    auto email = ui->email_edit->text();
    auto passwd = ui->passwd_edit->text();
    QString salt = "salt_" + email;
    QByteArray bytes = (passwd + salt).toUtf8();
    QByteArray hash = QCryptographicHash::hash(bytes, QCryptographicHash::Sha256);
    QString passwd_res = hash.toHex(); // 转成字符串存库

    QJsonObject json_obj;
    json_obj["email"] = email;
    json_obj["passwd"] = passwd_res;

    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),
                                        json_obj, ReqId::ID_USER_LOGIN, Modules::LOGINMOD);
}

void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if (err != ErrorCodes::SUCCESS) {
        showTip(tr("网络请求错误"), false);
        return;
    }

    // 解析返回的 json 串
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if (jsonDoc.isEmpty() || !jsonDoc.isObject()) {
        showTip(tr("json解析失败"), false);
        return;
    }

    handlers_[id](jsonDoc.object());
    return;
}

