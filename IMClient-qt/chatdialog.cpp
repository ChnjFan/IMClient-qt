#include "chatdialog.h"
#include "ui_chatdialog.h"

#include <QAction>
#include <QThread>

#include "clickedbtn.h"
#include "chatuserwid.h"
#include "chatuserlist.h"
#include "loadingdialog.h"

ChatDialog::ChatDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatDialog),
    state_(ChatUIMode::ChatMode),
    mode_(ChatUIMode::ChatMode),
    loading_(false)
{
    ui->setupUi(this);
    ui->add_btn->SetState("normal", "hover", "press");
    ui->search_edit->SetMaxLength(15);

    QAction *searchAction = new QAction(ui->search_edit);
    searchAction->setIcon(QIcon(":/resource/icon/search.svg"));
    ui->search_edit->addAction(searchAction, QLineEdit::LeadingPosition);
    ui->search_edit->setPlaceholderText(QStringLiteral("搜索"));

    QAction *clearAction = new QAction(ui->search_edit);
    clearAction->setIcon(QIcon(":/resource/icon/delete.svg"));
    ui->search_edit->addAction(clearAction, QLineEdit::TrailingPosition);

    // 只有文本输入时显示清除图标
    connect(ui->search_edit, &QLineEdit::textChanged, [this, clearAction](const QString &text){
        if (!text.isEmpty()) {
            clearAction->setIcon(QIcon(":/resource/icon/delete_fill.svg"));
            showSearch(true);
        }
        else {
            clearAction->setIcon(QIcon(":/resource/icon/delete.svg"));
            showSearch(false);
        }
    });

    // 清除动作触发信号
    connect(clearAction, &QAction::triggered, [this, clearAction](){
        ui->search_edit->clear();
        clearAction->setIcon(QIcon(":/resource/icon/delete.svg"));
        ui->search_edit->clearFocus();
        // 关闭搜索框
        showSearch(false);
    });

    showSearch(false);

    connect(ui->chat_user_list, &ChatUserList::sig_loading_chat_user, this, &ChatDialog::slot_loading_chat_user);
    addChatUserList();

    QPixmap pixmap(":/resource/avatar/boy_yellow.svg");
    QPixmap scaledPixmap = pixmap.scaled(ui->side_head_lb->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation); // 将图片缩放到label的大小
    ui->side_head_lb->setPixmap(pixmap);
    ui->side_head_lb->setScaledContents(true); // 设置QLabel自动缩放图片内容以适应大小

    ui->side_chat_lb->setProperty("state", "normal");
    ui->side_chat_lb->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed");

    ui->side_contact_lb->SetState("normal", "hover", "pressed", "selected_normal", "selected_hover", "selected_pressed");


}

ChatDialog::~ChatDialog()
{
    delete ui;
}

void ChatDialog::showSearch(bool search)
{
    if (search) {   // 搜索框，将会话和联系人界面隐藏，显示搜索列表
        ui->chat_user_list->hide();
        ui->con_user_list->hide();
        ui->search_user_list->show();
        mode_ = ChatUIMode::SearchMode;
    }
    else if (state_ == ChatUIMode::ChatMode) {
        ui->con_user_list->hide();
        ui->search_user_list->hide();
        ui->chat_user_list->show();
        mode_ = ChatUIMode::ChatMode;
    }
    else if (state_ == ChatUIMode::ContactMode) {
        ui->search_user_list->hide();
        ui->chat_user_list->hide();
        ui->con_user_list->show();
        mode_ = ChatUIMode::ContactMode;
    }
}

void ChatDialog::addChatUserList()
{
    for (int i = 0; i < 2; ++i) {
        auto *chatUserWid = new ChatUserWid();
        chatUserWid->SetInfo("测试用户-男", ":/resource/avatar/boy_blue.svg", "男生历史消息");
        auto *item = new QListWidgetItem;
        item->setSizeHint(chatUserWid->sizeHint());
        ui->chat_user_list->addItem(item);
        ui->chat_user_list->setItemWidget(item, chatUserWid);

        auto *chatUserWid1 = new ChatUserWid();
        chatUserWid1->SetInfo("测试用户-女", ":/resource/avatar/gril_pink.svg", "女生历史消息长度超过限制长度超过限制长度超过限制");
        auto *item1 = new QListWidgetItem;
        item1->setSizeHint(chatUserWid1->sizeHint());
        ui->chat_user_list->addItem(item1);
        ui->chat_user_list->setItemWidget(item1, chatUserWid1);
    }
}

void ChatDialog::AddLBGroup(StateWidget *lb)
{
    _lb_list.push_back(lb);
}

void ChatDialog::slot_loading_chat_user()
{
    if (loading_) {
        return;
    }

    loading_ = true;
    // 模态对话框，阻塞父窗口操作直到模态对话框关闭
    LoadingDialog *loadingDialog = new LoadingDialog(this);
    loadingDialog->setModal(true);
    loadingDialog->show();
    addChatUserList();
    // QThread::sleep(3);
    // 关闭对话框返回到父窗口
    loadingDialog->deleteLater();
    loading_ = false;
}

