#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>

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

private:
    void initHttpHandlers();
    void showTip(QString str, bool is_ok);

    Ui::RegisterDialog *ui;
    QMap<ReqId, std::function<void(const QJsonObject&)>> handlers_; // 处理函数
};

#endif // REGISTERDIALOG_H
