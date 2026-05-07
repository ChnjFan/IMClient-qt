#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 构造指定为MainWindow的子类，MainWindow会回收子类内存
    login_dialog_ = new LoginDialog(this);
    // 登录界面加载到主界面
    setCentralWidget(login_dialog_);
    // login_dialog_->show();

    // 注册界面初始化，创建和注册消息连接
    connect(login_dialog_, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    reg_dialog_ = new RegisterDialog(this);

    // 隐藏边框让界面嵌入主界面
    login_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    reg_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    reg_dialog_->hide();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchReg()
{
    // 切换界面，设置为注册对话
    setCentralWidget(reg_dialog_);
    login_dialog_->hide();// 隐藏登录界面
    reg_dialog_->show();
}
