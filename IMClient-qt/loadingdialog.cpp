#include "loadingdialog.h"
#include "ui_loadingdialog.h"

#include <QMovie>

LoadingDialog::LoadingDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadingDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明，避免遮挡父窗口
    setFixedSize(parent->size());   // 设置固定大小，与父窗口大小一致

    QMovie *movie = new QMovie(":/resource/icon/refresh.svg");
    ui->loading_lb->setMovie(movie);
    movie->start();
}

LoadingDialog::~LoadingDialog()
{
    delete ui;
}
