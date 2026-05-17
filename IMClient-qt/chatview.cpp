#include "chatview.h"

#include <QScrollBar>
#include <QEvent>
#include <QStyleOption>
#include <QPainter>

ChatView::ChatView(QWidget *parent) : QWidget(parent), isAppended(false)
{
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    pMainLayout->setContentsMargins(0, 0, 0, 0);    // 设置布局边框

    // 设置滚动区域
    pScrollArea_ = new QScrollArea();
    pScrollArea_->setObjectName("chat_area");
    pMainLayout->addWidget(pScrollArea_);

    QWidget *w = new QWidget(this);
    w->setObjectName("chat_bg");
    w->setAutoFillBackground(true);//自动填充背景
    // 创建子布局
    QVBoxLayout *pHLayout_1 = new QVBoxLayout();
    pHLayout_1->addWidget(new QWidget(), 100000);   // 在子布局中又添加一个 widget
    w->setLayout(pHLayout_1);
    pScrollArea_->setWidget(w);    //应该时在QSCrollArea构造后执行 才对

    pScrollArea_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar *pVScrollBar = pScrollArea_->verticalScrollBar();
    connect(pVScrollBar, &QScrollBar::rangeChanged,this, &ChatView::onVScrollBarMoved);// 显示区域发生变化

    //把垂直ScrollBar放到上边 而不是原来的并排
    QHBoxLayout *pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout_2->setContentsMargins(0,0,0,0);
    pScrollArea_->setLayout(pHLayout_2);
    pVScrollBar->setHidden(true);

    pScrollArea_->setWidgetResizable(true);
    pScrollArea_->installEventFilter(this);

}

void ChatView::appendChatItem(QWidget *item)
{
    QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(pScrollArea_->widget()->layout());
    qDebug() << "vl->count() is " << vl->count();
    vl->insertWidget(vl->count()-1, item);
    isAppended = true;
}

void ChatView::prependChatItem(QWidget *item)
{

}

void ChatView::insertChatItem(QWidget *before, QWidget *item)
{

}

void ChatView::removeAllItem()
{

}

bool ChatView::eventFilter(QObject *o, QEvent *e)
{
    if(e->type() == QEvent::Enter && o == pScrollArea_)
    {
        pScrollArea_->verticalScrollBar()->setHidden(pScrollArea_->verticalScrollBar()->maximum() == 0);
    }
    else if(e->type() == QEvent::Leave && o == pScrollArea_)
    {
        pScrollArea_->verticalScrollBar()->setHidden(true);
    }
    return QWidget::eventFilter(o, e);
}

void ChatView::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ChatView::onVScrollBarMoved(int min, int max)
{
    if(isAppended) //添加item可能调用多次
    {
        QScrollBar *pVScrollBar = pScrollArea_->verticalScrollBar();
        pVScrollBar->setSliderPosition(pVScrollBar->maximum());
        //500毫秒内可能调用多次
        QTimer::singleShot(500, [this]()
                           {
                               isAppended = false;
                           });
    }
}

void ChatView::initStyleSheet()
{

}
