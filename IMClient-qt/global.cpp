#include "global.h"

QString gate_url_prefix = "";

std::function<void(QWidget*)> repolish = [](QWidget* w) {
    // 卸载样式重新加载
    w->style()->unpolish(w);
    w->style()->polish(w);
};