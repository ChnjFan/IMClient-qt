#include "chatpage.h"
#include "ui_chatpage.h"

#include <QStyleOption>
#include <QPainter>
#include <QJsonObject>
#include <QJsonArray>

#include "usermgr.h"
#include "chatitembase.h"
#include "textbubble.h"
#include "picturebubble.h"

ChatPage::ChatPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChatDataWidget)
{
    ui->setupUi(this);

    ui->recv_btn->SetState("normal", "hover", "press");
    ui->send_btn->SetState("normal", "hover", "press");

    ui->emo_lb->setState("normal", "hover", "press", "normal", "hover", "press");
    ui->file_lb->setState("normal", "hover", "press", "normal", "hover", "press");

    ui->chat_edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    ui->chat_edit->setMinimumWidth(200);
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ChatPage::on_send_btn_clicked()
{
    auto pTextEdit = ui->chat_edit;
    ChatRole role = ChatRole::Self;
    QString userName = QStringLiteral("Admin");
    QString userIcon = ":/resource/avatar/boy_cyan.svg";

    const QVector<MsgInfo>& msgList = pTextEdit->getMsgList();
    for (int i = 0; i < msgList.size(); ++i) {
        QString type = msgList[i].msgFlag;
        ChatItemBase *pChatItem = new ChatItemBase(role);
        pChatItem->setUserIcon(QPixmap(userIcon));
        pChatItem->setUserName(userName);
        QWidget *pBubble = nullptr;
        if (type == "text") {
            pBubble = new TextBubble(role, msgList[i].content);
        }
        else if (type == "image") {
            pBubble = new PictureBubble(QPixmap(msgList[i].content), role);
        }

        if (nullptr != pBubble) {
            pChatItem->setWidget(pBubble);
            ui->chat_data_list->appendChatItem(pChatItem);
        }
    }
}

