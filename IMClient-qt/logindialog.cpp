#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    // 连接按钮事件
    connect(ui->reg_btn, &QPushButton::clicked, this, &LoginDialog::switchRegister);

    // 忘记密码设置状态
    ui->forget_label->setState("normal", "hover", "", "select", "select_hover", "");
    connect(ui->forget_label, &ClickedLabel::clicked, this, &LoginDialog::slot_forget_passwd);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::slot_forget_passwd()
{
    qDebug() << "Switch reset passwd";
    emit switchReset();
}
