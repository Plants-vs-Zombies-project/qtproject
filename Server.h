<<<<<<< HEAD
/*#pragma once
=======
#pragma once
>>>>>>> refs/remotes/iptuxgit/udp
#include <QHostAddress>
#include <QMap>
#include <QObject>
#include <QtNetwork/QUdpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

public slots:
    void processPendingDataGrams();

signals:
    void messageReceived(const QString &sender, const QString &message);
    void clientConnected(const QString &clientAddress);
    void clientDisconnected(const QString &clientAddress);

<<<<<<< HEAD
    private slots:
=======
private slots:
>>>>>>> refs/remotes/iptuxgit/udp
    void handleClientLeft(const QString &clientAddress);

private:
    void forwardMessage(const QString &sender, const QString &message);

private:
    QUdpSocket *udpSocket;
    QMap<QString, QHostAddress>
        clients; //restore estabilished client's ip Address and QHostAddress of object
};
<<<<<<< HEAD
*/
=======
>>>>>>> refs/remotes/iptuxgit/udp
