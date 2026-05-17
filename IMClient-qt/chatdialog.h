#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QWidget>

#include "global.h"
#include "statewidget.h"

namespace Ui {
class ChatDialog;
}

class ChatDialog : public QWidget
{
    Q_OBJECT

public:
    explicit ChatDialog(QWidget *parent = nullptr);
    ~ChatDialog();

private:
    void showSearch(bool search = false);
    void addChatUserList();

    void AddLBGroup(StateWidget* lb);

    Ui::ChatDialog *ui;
    ChatUIMode state_;
    ChatUIMode mode_;
    bool loading_;

    QList<StateWidget*> _lb_list;

public slots:
    void slot_loading_chat_user();
};

#endif // CHATDIALOG_H
