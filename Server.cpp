#include <Server.h>
//server
Server::Server(QObject *parent)
    : QObject(parent)
{
    //create socket
    udpSocket = new QUdpSocket(this);

    //bind to ip and port,unlike TCP,UDP doesn't need listen
    udpSocket->bind(QHostAddress::Any, 12345);

    connect(udpSocket,
            &QUdpSocket::readyread,
            this,
            &Server::processPendingDatagrams()); //using "progressPendingDtagrame" to read datagrame
}
//
void Server::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize()); //set datagrame's length
        //sender
        QHostAddress senderAddress; //ip of sender
        quint16 senderPort;  //sender of port

        //read datagrame and store ip and port message of client
        udpSocket->readDatagram(datagrame.data(), datagram.size(), &senderAddress, &senderPort);

        QString message = QString::fromUtf8(datagram);

        //if client doesn't exist in clients,store it and  emit signals  which has already connected sucessfully.
        if (!clients.contains(senderAddress.toString())) {
            clients.insert(senderAddress.toString(), senderAddress);
            emit clientConnected(senderAddress.toString());
        }

        if (message.startWith("/connect")) {
            QString parts = message.split(" ");
            if (parts.length() == 3 && parts[0] == "/connect") {
                QString recipientAddress = parts[1]; //connectToClient() of Client
                QString messageToSend = parts[2];    //client1 will send this message to client2
                forwardMessage(recipientAddress, messageToSend);
            }
        }
        //sucessed in receving message from client
        emit messageReceived(senderAddress.toString(), message);
    }
}

void Server::forwardMessage(const QString &recipientAddress, const QString &message)
{
    if (clients.contains(recipientAddress)) {
        QHostAddress recipient = clients.value(recipientAddress);
        QByteArray datagram = message.toUtf8();
        udpSocket->writeDatagram(datagram, recipient, 12345);
    }
}
void Server::handleClientLeft(const QSTring &clientAddress)
{
    if (clients.contains(clientAddress)) {
        clients.remove(clientAddress);
        emit clientDisconnected(clientAddress);
    }
}
