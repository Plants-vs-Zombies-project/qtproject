#pragma once
#include <QObject>
#include <QUdpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

public slots:
    void sendMessage(const QString &message);
    void sendImage(const QString &imagePath);
    void processPendingDatagrams();
    void onMessageReceived(const QString &message);
    void onImageReceived(const QByteArray &imageData);
    //void connectToClient(const QString &clientAddress);

signals:

    void messageReceived(const QString &message);
    void imageReceived(const QByteArray &imageData);

private:
    QUdpSocket *udpSocket;
};
