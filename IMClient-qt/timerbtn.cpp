#include "timerbtn.h"

#include <QMouseEvent>
#include <QDebug>

TimerBtn::TimerBtn(QWidget *parent) : counter_(10)
{
    timer_ = new QTimer(this);

    connect(timer_, &QTimer::timeout, [this](){
        counter_--;
        if (counter_ <= 0) {
            timer_->stop();
            counter_ = 10; // 重置计时次数
            this->setText("获取");
            this->setEnabled(true);
            return;
        }
        this->setText(QString::number(counter_));
    });
}

TimerBtn::~TimerBtn()
{
    timer_->stop();
}

void TimerBtn::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        // 鼠标左键事件
        qDebug() << "Button was released.";
        this->setEnabled(false); // 按钮不可用
        this->setText(QString::number(counter_));
        timer_->start(1000); // 每秒触发
        emit clicked(); // 发按钮点击信号
    }
    // 调用基类方法确保其他事件的处理
    QPushButton::mouseReleaseEvent(e);
}
