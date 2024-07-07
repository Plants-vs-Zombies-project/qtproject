#pragma once
#include <QObject>
#include <QUdpSocket>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

public slots:
    Q_INVOKABLE void sendMessage(const QString &message);
    Q_INVOKABLE void sendImage(const QString &imagePath);
    Q_INVOKABLE void processPendingDatagrams();
    Q_INVOKABLE void onMessageReceived(const QString &message);
    Q_INVOKABLE void onImageReceived(const QByteArray &imageData);
    //void connectToClient(const QString &clientAddress);

signals:

    void messageReceived(const QString &message);
    void imageReceived(const QByteArray &imageData);

private:
    QUdpSocket *udpSocket;
};
