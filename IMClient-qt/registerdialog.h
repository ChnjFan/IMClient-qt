#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QTimer>

#include "global.h"

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_get_code_clicked();
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_sure_btn_clicked();

    void on_return_btn_clicked();

    void on_cancel_btn_clicked();

private:
    void initHttpHandlers();
    void showTip(QString str, bool is_ok);

    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);

    bool checkUserValid();
    bool checkEmailValid();
    bool checkPasswdValid();
    bool checkConfirmValid();
    bool checkVerifyValid();

    void changeRegSuccessTipsPage();

    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> handlers_; // 处理函数
    QMap<TipErr, QString> tip_error_;

    QTimer *return_timer_;  // 页面返回倒计时
    int return_count_;

signals:
    void sigSwitchLogin(void);
};

#endif // REGISTERDIALOG_H
