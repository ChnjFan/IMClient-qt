#include "chatuserwid.h"
#include "ui_chatuserwid.h"

ChatUserWid::ChatUserWid(QWidget *parent)
    : ListItemBase(parent)
    , ui(new Ui::ChatUserWid)
{
    ui->setupUi(this);
    SetItemType(ListItemType::CHAT_USER_ITEM);
}

ChatUserWid::~ChatUserWid()
{
    delete ui;
}

QSize ChatUserWid::sizeHint() const
{
    return QSize(250,70);   // 返回尺寸
}

void ChatUserWid::SetInfo(QString name, QString head, QString msg)
{
    name_ = name;
    head_ = head;
    msg_ = msg;
    // 加载头像
    QPixmap pixmap(head_);
    ui->icon_lb->setPixmap(pixmap.scaled(ui->icon_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->icon_lb->setScaledContents(true);

    ui->name_lb->setText(name_);
    ui->chat_lb->setText(msg_);
    ui->time_lb->setText(QString("13:55"));
}
