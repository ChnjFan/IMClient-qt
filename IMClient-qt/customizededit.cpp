#include "customizededit.h"

CustomizedEdit::CustomizedEdit(QWidget *parent) : maxLen_(0)
{
    connect(this, &QLineEdit::textChanged, this, &CustomizedEdit::limitTextLength);
}

void CustomizedEdit::SetMaxLength(int maxLen)
{
    maxLen_ = maxLen;
}

void CustomizedEdit::focusOutEvent(QFocusEvent *)
{
    emit sig_foucus_out();
}

void CustomizedEdit::limitTextLength(QString text)
{
    if (maxLen_ <= 0) {
        return;
    }

    QByteArray byteArr = text.toUtf8();
    if (byteArr.size() > maxLen_) {
        byteArr = byteArr.left(maxLen_);
        this->setText(QString::fromUtf8(byteArr));
    }
}
