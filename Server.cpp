<<<<<<< HEAD
/*#include <Server.h>
=======
#include <Server.h>
>>>>>>> refs/remotes/iptuxgit/udp
//server
Server::Server(QObject *parent)
    : QObject(parent)
{
<<<<<<< HEAD
    V
        //create socket
        udpSocket
        = new QUdpSocket(this);
=======
    //create socket
    udpSocket = new QUdpSocket(this);
>>>>>>> refs/remotes/iptuxgit/udp

    //bind to ip and port,unlike TCP,UDP doesn't need listen
    udpSocket->bind(QHostAddress::Any, 12345);

    connect(udpSocket,
            &QUdpSocket::readyread,
            this,
            &Server::processPendingDatagrams()); //using "progressPendingDtagrame" to read datagrame
}
<<<<<<< HEAD

//send massage
void Client::sendMessge(const QString &message)
{
    QByteArray datagrams = message.toUtf8();
    cudpSocket->writeDatagram(datagrams, QHostAddress::LocalHost, 12345); //send message to server
}
//send image
void Client::sendImage(const QString &filePath)
{
    QFile file(&filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file" << file.errorString();
        return;
    }
    QByteArray imageData = file.readAll();
    cudpSocket->writeDatagram(imageData, QHostAddress::Any, 12346);
}

=======
>>>>>>> refs/remotes/iptuxgit/udp
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

<<<<<<< HEAD
        /*if (message.startWith("/connect")) {
=======
        if (message.startWith("/connect")) {
>>>>>>> refs/remotes/iptuxgit/udp
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
<<<<<<< HEAD
}*/

/*void Server::forwardMessage(const QString &recipientAddress, const QString &message)
=======
}

void Server::forwardMessage(const QString &recipientAddress, const QString &message)
>>>>>>> refs/remotes/iptuxgit/udp
{
    if (clients.contains(recipientAddress)) {
        QHostAddress recipient = clients.value(recipientAddress);
        QByteArray datagram = message.toUtf8();
        udpSocket->writeDatagram(datagram, recipient, 12345);
    }
}
<<<<<<< HEAD
void Server::handleClientLeft(const QString &clientAddress)
=======
void Server::handleClientLeft(const QSTring &clientAddress)
>>>>>>> refs/remotes/iptuxgit/udp
{
    if (clients.contains(clientAddress)) {
        clients.remove(clientAddress);
        emit clientDisconnected(clientAddress);
    }
<<<<<<< HEAD
}*/
=======
}
>>>>>>> refs/remotes/iptuxgit/udp
