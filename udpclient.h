#pragma once
#include <QObject>
#include <QUdpSocket>
class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

public slots:
    void sendMessage(const QString &messgae);
    void processPendingDatagrams();
    void connectToClient(const QString &clientAddress);

signals:
    void messageReceived(const QString &sender, const Qstring &message);

private:
    QUdpSocket *cudpSocket;
    QString serverAddress;
};
