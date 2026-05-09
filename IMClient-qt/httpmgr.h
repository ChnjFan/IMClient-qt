#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QString>
#include <QUrl>
#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QJsonObject>
#include <QJsonDocument>

#include "singleton.h"

class HttpMgr : public QObject, public Singleton<HttpMgr>, public std::enable_shared_from_this<HttpMgr>
{
    Q_OBJECT

public:
    ~HttpMgr();

    void PostHttpReq(QUrl url, QJsonObject json, ReqId reqId, Modules mod);

private:
    friend class Singleton<HttpMgr>; // 声明友元让基类可以访问子类的构造函数
    HttpMgr();

    QNetworkAccessManager manager_;

private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);

signals:
    // http 发送完成信号通知其他模块
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    // 注册完成信号
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
    void sig_reset_mod_finish(ReqId id, QString res, ErrorCodes err);
};

#endif // HTTPMGR_H
