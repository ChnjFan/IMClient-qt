#include "clickedlabel.h"

ClickedLabel::ClickedLabel(QWidget *parent) : QLabel(parent), state_(ClickLbState::Normal)
{

}

void ClickedLabel::mousePressEvent(QMouseEvent *ev)
{
    if (ev->button() == Qt::LeftButton) { //只做鼠标左键处理
        if (state_ == ClickLbState::Normal) { // 默认隐藏，切换到显示
            qDebug() << "clicked, change to select hover: " << select_hover_;
            state_ = ClickLbState::Select;
            setProperty("state", select_hover_);
            repolish(this);
            update();
        }
        else {
            qDebug() << "clicked, change to normal hover: " << normal_hover_;
            state_ = ClickLbState::Normal;
            setProperty("state", normal_hover_);
            repolish(this);
            update();
        }
        emit clicked();
    }
    QLabel::mousePressEvent(ev);
}

void ClickedLabel::enterEvent(QEnterEvent *event)
{
    if (state_ == ClickLbState::Normal) {
        qDebug() << "enter, change to normal hover: " << normal_hover_;
        setProperty("state", normal_hover_);
        repolish(this);
        update();
    }
    else {
        qDebug() << "enter, change to select hover: " << select_hover_;
        setProperty("state", select_hover_);
        repolish(this);
        update();
    }
}

void ClickedLabel::leaveEvent(QEvent *event)
{
    if (state_ == ClickLbState::Normal) {
        qDebug() << "leave, change to normal: " << normal_;
        setProperty("state", normal_);
        repolish(this);
        update();
    }
    else {
        qDebug() << "leave, change to select hover: " << select_;
        setProperty("state", select_);
        repolish(this);
        update();
    }
}

void ClickedLabel::setState(QString normal, QString hover, QString press, QString select, QString select_hover, QString select_press)
{
    normal_ = normal;
    normal_hover_ = hover;
    normal_press_ = press;

    select_ = select;
    select_hover_ = select_hover;
    select_press_ = select_press;

    setProperty("state", normal);
    repolish(this);
}

ClickLbState ClickedLabel::getCurState()
{
    return state_;
}
