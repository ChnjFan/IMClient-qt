#include "tcpmgr.h"

#include <QAbstractSocket>
#include <QJsonObject>
#include <QJsonDocument>

#include "usermgr.h"

TcpMgr::~TcpMgr()
{

}

TcpMgr::TcpMgr() : host_(""), port_(0), recv_pending(false), message_id_(0), message_len_(0)
{
    QObject::connect(&socket_, &QTcpSocket::connected, [&](){
        qDebug() << "Connect to tcp server";
        emit sig_conn_success(true);
    });

    QObject::connect(&socket_, &QTcpSocket::readyRead, [&](){
        buffer_.append(socket_.readAll());
        QDataStream stream(&buffer_, QIODevice::ReadOnly);
        stream.setByteOrder(QDataStream::BigEndian);// 网络序
        stream.setVersion(QDataStream::Qt_5_0);

        forever {
            // 检查消息处理 TCP 粘包
            if (!recv_pending) {
                // 解析 4 字节报文头，消息 ID 和长度
                if (buffer_.length() < static_cast<int>(sizeof(quint16) * 2)) {
                    return;
                }
                // QDataStream 在写入数据时强制转换为大端网络序，所以读的时候也是自动从大端序转换回来
                stream >> message_id_ >> message_len_;
                buffer_ = buffer_.mid(sizeof(quint16) * 2); // 将报文头移除
                qDebug() << "Recv tcp msg: id = " << message_id_ << "len = " << message_len_;
            }

            if (buffer_.length() < message_len_) {
                recv_pending = true;
                return;
            }

            recv_pending = false;
            QByteArray body = buffer_.mid(0, message_len_); // 取数据，不会将 buffer_ 中的数据清除
            qDebug() << "Recv tcp body: " << body;
            buffer_ = buffer_.mid(message_len_);
            handleMsg(ReqId(message_id_), message_len_, body);
        }
    });

    // 网络异常处理
    QObject::connect(&socket_, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError){
        Q_UNUSED(socketError);
        qDebug() << "Error: " << socket_.errorString();
    });

    // 连接断开
    QObject::connect(&socket_, &QTcpSocket::disconnected, [&](){
        qDebug() << "Disconnected from server";
    });
    // 发送数据信号
    QObject::connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);
    initHandlers();
}

void TcpMgr::initHandlers()
{
    handlers_.insert(ID_CHAT_LOGIN_RSP, [this](ReqId id, int len, QByteArray data){
        Q_UNUSED(len);
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        if (jsonDoc.isNull()) {
            qDebug() << "Recv null data";
            return;
        }

        QJsonObject jsonObj = jsonDoc.object();
        if (!jsonObj.contains("error")) {
            int err = ErrorCodes::ERR_JSON;
            qDebug() << "Chatserver login fail: " << err;
            emit sig_login_failed(err);
            return;
        }

        int err = jsonObj["error"].toInt();
        if (err != ErrorCodes::SUCCESS) {
            qDebug() << "Chatserver login fail: " << err;
            emit sig_login_failed(err);
            return;
        }

        qDebug() << "Chatserver login user: " << jsonObj["name"].toString();
        // 保存用户登录信息
        UserMgr::GetInstance()->setUid(jsonObj["uid"].toInt());
        UserMgr::GetInstance()->setName(jsonObj["name"].toString());
        UserMgr::GetInstance()->setToken(jsonObj["token"].toString());

        emit sig_switch_chat_dialog();

    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto iter = handlers_.find(id);
    if (iter == handlers_.end()) {
        return;
    }
    iter.value()(id, len, data);
}

void TcpMgr::slot_tcp_connect(ServerInfo serverInfo)
{
    // 用户登录成功后，获取到服务器地址进行连接
    host_ = serverInfo.host;
    port_ = static_cast<uint16_t>(serverInfo.port.toUInt());
    qDebug() << "Connect to " << host_ << ":" << port_;
    socket_.connectToHost(host_, port_);
}

void TcpMgr::slot_send_data(ReqId id, QString data)
{
    quint16 msgId = id;
    QByteArray dataBytes = data.toUtf8();
    quint16 len = static_cast<quint16>(data.size());

    // 要发送的数据
    QByteArray block;
    QDataStream stream(&block, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::BigEndian);// 网络序

    stream << msgId << len;
    block.append(dataBytes);
    socket_.write(block);
}
