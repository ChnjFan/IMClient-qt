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
    login_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // 注册界面初始化，创建和注册消息连接
    connect(login_dialog_, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    // 连接重置密码界面信号
    connect(login_dialog_, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotSwitchReg()
{
    // 切换界面，设置为注册对话
    // 切换时再申请内存
    reg_dialog_ = new RegisterDialog(this);

    // 隐藏边框让界面嵌入主界面
    reg_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    setCentralWidget(reg_dialog_);
    login_dialog_->hide();// 隐藏登录界面
    reg_dialog_->show();

    // 连接注册成功返回登录信号
    connect(reg_dialog_, &RegisterDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);
}

void MainWindow::SlotSwitchLogin()
{
    // 登录界面切换后会被析构，切换回来要重新申请内存
    login_dialog_ = new LoginDialog(this);
    // 登录界面加载到主界面
    setCentralWidget(login_dialog_);
    // login_dialog_->show();
    login_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    // 注册界面初始化，创建和注册消息连接
    connect(login_dialog_, &LoginDialog::switchRegister, this, &MainWindow::SlotSwitchReg);
    connect(login_dialog_, &LoginDialog::switchReset, this, &MainWindow::SlotSwitchReset);
}

void MainWindow::SlotSwitchReset()
{
    reset_diaglog_ = new ResetDialog(this);
    reset_diaglog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    setCentralWidget(reset_diaglog_);
    login_dialog_->hide();// 隐藏登录界面
    reset_diaglog_->show();

    // 重置成功返回信号
    connect(reset_diaglog_, &ResetDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);
}
