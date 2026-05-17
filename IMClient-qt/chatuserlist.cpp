#include "chatuserlist.h"

#include <QEvent>
#include <QWheelEvent>
#include <QScrollBar>

#include "chatuserwid.h"

ChatUserList::ChatUserList(QWidget *parent) : QListWidget(parent)
{
    // 关闭滚动事件
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
    this->viewport()->installEventFilter(this);
}

bool ChatUserList::eventFilter(QObject *watched, QEvent *event)
{
    // 检查事件是否为鼠标悬浮进入
    if (watched == this->viewport()) {
        if (event->type() == QEvent::Enter) {// 鼠标进入用户列表中显示滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }
        else if (event->type() == QEvent::Leave) {
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
    // 鼠标滚轮滚动事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 2;
        int numSteps = numDegrees / 1;// 计算滚动步数

        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        // 检查是不是滚到底部
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int curValue = scrollBar->value();
        if (maxScrollValue - curValue <= 0) {
            qDebug() << "加载新的联系人";
            emit sig_loading_chat_user();
        }

        return true;
    }

    return QListWidget::eventFilter(watched, event);
}

