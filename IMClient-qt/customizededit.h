#ifndef CUSTOMIZEDEDIT_H
#define CUSTOMIZEDEDIT_H

#include <QLineEdit>

class CustomizedEdit : public QLineEdit
{
    Q_OBJECT
public:
    CustomizedEdit(QWidget *parent = nullptr);
    void SetMaxLength(int maxLen);

protected:
    void focusOutEvent(QFocusEvent *) override;

private:
    void limitTextLength(QString text);

    int maxLen_;

signals:
    void sig_foucus_out();
};

#endif // CUSTOMIZEDEDIT_H
