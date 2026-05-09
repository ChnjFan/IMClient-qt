#include <QJsonObject>
#include <QUrl>
#include <QCryptographicHash>

#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "httpmgr.h"
#include "glob.h"

RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    ui->passwd_edit->setEchoMode(QLineEdit::Password);
    ui->ack_edit->setEchoMode(QLineEdit::Password);
    // 设置err_tip属性，刷新界面
    ui->err_tip->setProperty("state", "normal");
    repolish(ui->err_tip);

    // 连接http注册完成的信号
    connect(HttpMgr::GetInstance().get(), &HttpMgr::sig_reg_mod_finish,
            this, &RegisterDialog::slot_reg_mod_finish);
    // 注册处理函数
    initHttpHandlers();
    ui->err_tip->clear();   // 界面切换进来不显示错误提示

    // 输入每个对话框都要检查正确性
    connect(ui->user_edit, &QLineEdit::editingFinished, this, [this](){
        checkUserValid();
    });
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });
    connect(ui->passwd_edit, &QLineEdit::editingFinished, this, [this](){
        checkPasswdValid();
    });
    connect(ui->ack_edit, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });
    connect(ui->verify_edit, &QLineEdit::editingFinished, this, [this](){
        checkVerifyValid();
    });

    // 加载密码显示图标
    ui->passwd_visible->setState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");
    ui->ack_visible->setState("unvisible", "unvisible_hover", "", "visible", "visible_hover", "");

    // 密码显示鼠标悬浮效果
    ui->passwd_visible->setCursor(Qt::PointingHandCursor);
    ui->ack_visible->setCursor(Qt::PointingHandCursor);

    // 绑定密码输入显示模式的信号
    connect(ui->passwd_visible, &ClickedLabel::clicked, this, [this](){
        auto state = ui->passwd_visible->getCurState();
        if (state == ClickLbState::Normal) {
            ui->passwd_edit->setEchoMode(QLineEdit::Password);
        }
        else {
            ui->passwd_edit->setEchoMode(QLineEdit::Normal);
        }
    });

    connect(ui->ack_visible, &ClickedLabel::clicked, this, [this](){
        auto state = ui->ack_visible->getCurState();
        if (state == ClickLbState::Normal) {
            ui->ack_edit->setEchoMode(QLineEdit::Password);
        }
        else {
            ui->ack_edit->setEchoMode(QLineEdit::Normal);
        }
    });
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_get_code_clicked()
{
    auto email = ui->email_edit->text();
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

    if (regex.match(email).hasMatch()) {
        // 匹配邮箱后发送验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix + "/get_verify_code"),
                                            json_obj, ReqId::ID_GET_VERIFY_CODE, Modules::REGISTERMOD);
    }
    else {
        showTip(tr("邮箱地址不正确"), false);
    }
}

void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
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

void RegisterDialog::initHttpHandlers()
{
    // 验证码功能结果
    handlers_.insert(ReqId::ID_GET_VERIFY_CODE, [this](const QJsonObject& jsonObj) {
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("验证码发送失败"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("验证码已经发送到邮箱，请注意查收"), true);
        qDebug() << "email is " << email;
    });
    // 注册结果
    handlers_.insert(ReqId::ID_REG_USER, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if (error != ErrorCodes::SUCCESS) {
            showTip(tr("用户注册失败"), false);
            return;
        }

        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"), true);
        qDebug() << "email is " << email;
    });
}

void RegisterDialog::showTip(QString str, bool is_ok)
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

void RegisterDialog::AddTipErr(TipErr te, QString tips)
{
    tip_error_[te] = tips;
    showTip(tips, false);
}

void RegisterDialog::DelTipErr(TipErr te)
{
    tip_error_.remove(te);
    if (tip_error_.empty()) {
        ui->err_tip->clear(); // 所有错误提示完后清空界面
        return;
    }
    showTip(tip_error_.first(), false);
}

bool RegisterDialog::checkUserValid()
{
    if (ui->user_edit->text() == "") {
        AddTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }

    DelTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid()
{
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    auto email = ui->email_edit->text();

    if (!regex.match(email).hasMatch()) {
        AddTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱输入错误"));
        return false;
    }

    DelTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool RegisterDialog::checkPasswdValid()
{
    auto passwd = ui->passwd_edit->text();
    auto confirm = ui->ack_edit->text();

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

    if (passwd != confirm) {
        AddTipErr(TipErr::TIP_PWD_CONFIRM_ERR, tr("密码与确认密码不匹配"));
        return false;
    }
    else {
        DelTipErr(TipErr::TIP_PWD_CONFIRM_ERR);
    }

    return true;
}

bool RegisterDialog::checkConfirmValid()
{
    auto passwd = ui->passwd_edit->text();
    auto confirm = ui->ack_edit->text();

    if (passwd != confirm) {
        AddTipErr(TipErr::TIP_PWD_CONFIRM_ERR, tr("密码与确认密码不匹配"));
        return false;
    }
    else {
        DelTipErr(TipErr::TIP_PWD_CONFIRM_ERR);
    }

    return true;
}

bool RegisterDialog::checkVerifyValid()
{
    auto verify = ui->verify_edit->text();
    if (verify.length() != 4) {
        AddTipErr(TipErr::TIP_VERIFY_ERR, tr("验证码输入错误"));
        return false;
    }
    DelTipErr(TipErr::TIP_VERIFY_ERR);
    return true;
}

void RegisterDialog::on_sure_btn_clicked()
{
    QJsonObject json_obj;

    // 先检查密码和确认是否相同
    QString passwd = ui->passwd_edit->text();
    QString confirm = ui->ack_edit->text();
    if (0 == passwd.length()) {
        showTip(tr("请输入密码"), false);
        return;
    }
    if (passwd != confirm) {
        showTip(tr("用户密码确认不一致"), false);
        return;
    }

    QString user = ui->user_edit->text();
    if (user.length() == 0) {
        showTip(tr("请输入用户名"), false);
        return;
    }

    QString salt = "salt_" + user;
    QByteArray bytes = (passwd + salt).toUtf8();
    QByteArray hash = QCryptographicHash::hash(bytes, QCryptographicHash::Sha256);
    QString passwd_res = hash.toHex(); // 转成字符串存库

    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = passwd_res;
    json_obj["confirm"] = passwd_res;
    json_obj["verify_code"] = ui->verify_edit->text();

    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER, Modules::REGISTERMOD);
}

