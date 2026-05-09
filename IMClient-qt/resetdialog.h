#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>

#include "global.h"

namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);
    ~ResetDialog();

private:
    void initHttpHandlers();
    void AddTipErr(TipErr te, QString tips);
    void DelTipErr(TipErr te);

    void showTip(QString str, bool is_ok);

    bool checkUserValid();
    bool checkEmailValid();
    bool checkPasswdValid();
    bool checkConfirmValid();
    bool checkVerifyValid();

    Ui::ResetDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> handlers_; // 处理函数
    QMap<TipErr, QString> tip_error_;

signals:
    void sigSwitchLogin();
private slots:
    void on_cancel_btn_clicked();
    void on_sure_btn_clicked();
    void slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err);
    void on_get_code_clicked();
};

#endif // RESETDIALOG_H
