#include "clickedbtn.h"

ClickedBtn::ClickedBtn(QWidget *parent)
{
    setCursor(Qt::PointingHandCursor);
}

ClickedBtn::~ClickedBtn()
{

}

void ClickedBtn::SetState(QString normal, QString hover, QString press)
{
    normal_ = normal;
    hover_ = hover;
    press_ = press;
    setProperty("state", normal);
    repolish(this);
    update();
}

void ClickedBtn::enterEvent(QEnterEvent *event)
{
    setProperty("state", hover_);
    repolish(this);
    update();
    QPushButton::enterEvent(event);
}

void ClickedBtn::leaveEvent(QEvent *event)
{
    setProperty("state", normal_);
    repolish(this);
    update();
    QPushButton::leaveEvent(event);
}

void ClickedBtn::mousePressEvent(QMouseEvent *e)
{
    setProperty("state", press_);
    repolish(this);
    update();
    QPushButton::mousePressEvent(e);
}

void ClickedBtn::mouseReleaseEvent(QMouseEvent *e)
{
    setProperty("state", normal_);
    repolish(this);
    update();
    QPushButton::mouseReleaseEvent(e);
}
