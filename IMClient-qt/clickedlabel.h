#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H

#include <QLabel>
#include <QObject>
#include <QMouseEvent>
#include <QEnterEvent>
#include "global.h"

class ClickedLabel : public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget *parent=nullptr);
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    void setState(QString normal = "", QString hover = "", QString press = "",
                  QString select = "", QString select_hover = "", QString select_press = "");
    ClickLbState getCurState();

private:
    QString normal_;
    QString normal_hover_;
    QString normal_press_;

    QString select_;
    QString select_hover_;
    QString select_press_;

    ClickLbState state_;

signals:
    void clicked(void);
};

#endif // CLICKEDLABEL_H
