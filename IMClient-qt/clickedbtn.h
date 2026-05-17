#ifndef CLICKEDBTN_H
#define CLICKEDBTN_H

#include <QObject>
#include <QPushButton>

#include "global.h"

class ClickedBtn : public QPushButton
{
    Q_OBJECT
public:
    ClickedBtn(QWidget *parent = nullptr);
    ~ClickedBtn();

    void SetState(QString normal, QString hover, QString press);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

private:
    QString normal_;
    QString hover_;
    QString press_;

};

#endif // CLICKEDBTN_H
