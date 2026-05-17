#include "listitembase.h"

#include <QStyleOption>
#include <QPainter>

ListItemBase::ListItemBase(QWidget *parent) : type_(ListItemType::CHAT_USER_ITEM)
{

}

void ListItemBase::SetItemType(ListItemType type)
{
    type_ = type;
}


void ListItemBase::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
