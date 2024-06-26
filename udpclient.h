<<<<<<< HEAD
/*#pragma once
/*#include <QObject>
=======
#pragma once
#include <QObject>
>>>>>>> refs/remotes/iptuxgit/udp
#include <QUdpSocket>
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
<<<<<<< HEAD
    explicit ~Client();
=======

>>>>>>> refs/remotes/iptuxgit/udp
public slots:
    void sendMessage(const QString &messgae);
    void processPendingDatagrams();
    void connectToClient(const QString &clientAddress);
<<<<<<< HEAD
    void sendImage(const QString &filePath);
=======

>>>>>>> refs/remotes/iptuxgit/udp
signals:
    void messageReceived(const QString &sender, const Qstring &message);

private:
    QUdpSocket *cudpSocket;
    QString serverAddress;
};
<<<<<<< HEAD
*/
=======
>>>>>>> refs/remotes/iptuxgit/udp
