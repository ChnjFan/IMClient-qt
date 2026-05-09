#ifndef TIMERBTN_H
#define TIMERBTN_H

#include <QPushButton>
#include <QTimer>

class TimerBtn : public QPushButton
{
public:
    TimerBtn(QWidget *parent = nullptr);
    ~TimerBtn();

    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    QTimer *timer_; // 等待结束按钮重新可以按
    int counter_;
};

#endif // TIMERBTN_H
