#include "global.h"

QString gate_url_prefix = "";

std::function<void(QWidget*)> repolish = [](QWidget* w) {
    // 卸载样式重新加载
    w->style()->unpolish(w);
    w->style()->polish(w);
};

std::function<QString(QString)> xorString = [](QString input) {
    QString result = input;
    int length = result.length();
    length = length % 255;
    for (int i = 0; i < length; ++i) {
        // 对每个字符异或操作
        result[i] = QChar(static_cast<ushort>(input[i].unicode() ^ static_cast<ushort>(length)));
    }

    return result;
};