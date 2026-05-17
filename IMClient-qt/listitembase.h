#ifndef LISTITEMBASE_H
#define LISTITEMBASE_H

#include <QObject>
#include <QWidget>

#include "global.h"

// 列表项基类
class ListItemBase : public QWidget
{
    Q_OBJECT
public:
    explicit ListItemBase(QWidget *parent = nullptr);

    void SetItemType(ListItemType type);

protected:
    void paintEvent(QPaintEvent *event);
private:
    ListItemType type_;
};

#endif // LISTITEMBASE_H
