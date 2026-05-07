#include <QJsonObject>
#include <QUrl>

#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "httpmgr.h"

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
        //todo: 匹配邮箱后发送验证码
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
void RegisterDialog::on_sure_btn_clicked()
{
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = ui->passwd_edit->text();
    json_obj["confirm"] = ui->ack_edit->text();
    json_obj["verifycode"] = ui->verify_edit->text();

    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER, Modules::REGISTERMOD);
}

