#include <QtNetwork/QNetworkReply>

#include "httpmgr.h"

HttpMgr::~HttpMgr()
{

}

HttpMgr::HttpMgr()
{
    // 将信号与槽函数绑定，信号触发时槽函数自动执行
    connect(this, &HttpMgr::sig_http_finish, &HttpMgr::slot_http_finish);
}

void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId reqId, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);
    // http 请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));

    auto self = shared_from_this();
    // 发送 POST http 报文
    QNetworkReply *reply = manager_.post(request, data);
    // 接收 http 响应完成信号，不能直接捕获 this 指针，容易出现野指针
    QObject::connect(reply, &QNetworkReply::finished, [self, reply, reqId, mod](){
        // 错误情况
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << reply->errorString();
            // 发送信号给相关界面完成错误
            emit self->sig_http_finish(reqId, "", ErrorCodes::ERR_NETWORK, mod);
            reply->deleteLater();
            return;
        }

        QString res = reply->readAll();//读出所有字节
        emit self->sig_http_finish(reqId, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
    });

}

void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    if (mod == Modules::REGISTERMOD) {
        // 通知注册结果
        emit sig_reg_mod_finish(id, res, err);
    }
}
