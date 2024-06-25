#include <udpclient.h>

Client::Client(QObject *parent)
    : QObject(parent)
{
    cudpSocket = new QUdpSocket(this);

    connect(cudpSocket, &QUdpSocket::readyread, this, &Client::processPendingDatagrams());
}

void Client::sendMessge(const QString &messge)
{
    QByteArray datagrams = message.toUtf8();
    cudpSocket->writeDatagram(datagrams, QHostAddress::LocalHost, 12345); //send message to server
}

void Client::connectToClient(const QString &clientAddress)
{
    sendMessage("/connect" + clientAddress + "hello!");
}
void Client::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagrams;
        datagrams.resize(cudpSocket->pendingDatagramSize()); //set datagrame's length
        //sender
        QHostAddress senderAddress; //ip of sender
        quint16 senderPort;         //sender of port

        //read datagrame,receive message from server
        cudpSocket->readDatagram(datagrams.data(), datagrams.size(), &senderAddress, &senderPort);

        QString message = QString::fromUtf8(datagrams);
        //qDebug() << message;
        emit messageReceived(senderAddress.toString(), message);
    }
}
