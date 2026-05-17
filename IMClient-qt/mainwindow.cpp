#include "mainwindow.h"

#include <QSize>

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
    // 聊天界面切换
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_switch_chat_dialog, this, &MainWindow::SlotSwitchChat);

    // test: 聊天界面
    emit TcpMgr::GetInstance()->sig_switch_chat_dialog();
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
    reset_dialog_ = new ResetDialog(this);
    reset_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    setCentralWidget(reset_dialog_);
    login_dialog_->hide();// 隐藏登录界面
    reset_dialog_->show();

    // 重置成功返回信号
    connect(reset_dialog_, &ResetDialog::sigSwitchLogin, this, &MainWindow::SlotSwitchLogin);
}

void MainWindow::SlotSwitchChat()
{
    chat_dialog_ = new ChatDialog(this);
    // 设置窗口标志，去掉默认标题栏、边框，自定义窗口提示
    chat_dialog_->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    setCentralWidget(chat_dialog_);
    login_dialog_->hide();
    chat_dialog_->show();
    this->setMinimumSize(QSize(900, 500));
    this->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}
