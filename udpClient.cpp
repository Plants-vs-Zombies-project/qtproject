
#include "udpClient.h"
#include <QBuffer>
#include <QDataStream>
#include <QFile>
#include <QImage>
Client::Client(QObject *parent)
    : QObject(parent)
    , udpSocket(new QUdpSocket(this))
{
    //udpSocket->bind(QHostAddress::LocalHost, 12346);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Client::processPendingDatagrams);
}
void Client::sendMessage(const QString &message)
{
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out << QString("message") << message;
    udpSocket->writeDatagram(datagram, QHostAddress::LocalHost, 12346);
}

void Client::sendImage(const QString &imagePath)
{
    //打开图片文件
    QFile file(imagePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open image file:" << imagePath;
        return;
    }

    //读取图片文件的数据，并通过out向datagram写入图片数据，并发送
    QByteArray imageData = file.readAll();
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    out << QString("image") << imageData;

    udpSocket->writeDatagram(imageData, QHostAddress::LocalHost, 12346);
}

//接收数据的处理函数
void Client::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize()); //set datagrame's length

        QHostAddress senderAddress; //ip of sender
        quint16 senderPort;         //sender of port

        //读取数据
        udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress, &senderPort);

        //通过in从datagram中读取数据，将读到的消息类型存储在messageType中
        QDataStream in(&datagram, QIODevice::ReadOnly);
        QString messageType;
        in >> messageType;

        //判断消息类型，是发送的文本还是图片，是文本，就打印输出，是图片，就发送已经收到的信号
        if (messageType == "message") {
            QString text;
            in >> text;
            emit messageReceived(text);
        } else if (messageType == "image") {
            emit imageReceived(datagram);
        }
    }
}
//输出读到的文本消息，这里需要和头文件里声明的信号"messageReceived"连接起来使用（connect）
void Client::onMessageReceived(const QString &message)
{
    qDebug() << "REcevied message:" << message;
}
//和已声明的信号imageReceived连接起来使用(connect)
void Client::onImageReceived(const QByteArray &imageData)
{
    qDebug() << "Received image data:";
}
