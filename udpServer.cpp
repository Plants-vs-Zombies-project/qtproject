#include <QDataStream>
#include <QFile>
#include <udpServer.h>
//server
Server::Server(QObject *parent)
    : QObject(parent)
    , udpSocket(new QUdpSocket(this))
{
    //本地主机，端口号为12346
    udpSocket->bind(QHostAddress::LocalHost, 12346);
    connect(udpSocket, &QUdpSocket::readyRead, this, &Server::processPendingDatagrams);
}
//服务器打印开始运行的信息
void Server::startServer()
{
    qDebug() << "Server started on port 12346";
}
//发送文本消息
void Server::sendMessage(const QString &message)
{
    //创建一个QDataStream对象，并将其与datagram关联，我们就通过out向datagram写入数据
    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);
    //message和“message"文本标志将被写入到datagram中
    out << QString("message") << message;
    udpSocket->writeDatagram(datagram, QHostAddress::LocalHost, 12346);
}
//发送图片
void Server::sendImage(const QString &imagePath)
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
//读取数据的处理函数
void Server::processPendingDatagrams()
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
void Server::onMessageReceived(const QString &message)
{
    qDebug() << "Recevied message:" << message;
}

//和已声明的信号imageReceived连接起来使用(connect)
void Server::onImageReceived(const QByteArray &imageData)
{
    qDebug() << "Received image data:";
}
