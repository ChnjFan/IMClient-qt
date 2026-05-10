#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

#include "global.h"

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    void initHttpHandlers();

    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);
    void showTip(QString str, bool is_ok);

    bool checkUserValid();
    bool checkPasswdValid();

    Ui::LoginDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> handlers_;
    QMap<TipErr, QString> tip_error_;

private slots:
    void slot_forget_passwd();
    void on_login_btn_clicked();
    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes err);

    void slot_tcp_con_finish(bool success);

signals:
    void switchRegister();
    void switchReset();
    void sig_connect_tcp(ServerInfo info);

};

#endif // LOGINDIALOG_H
