#pragma once
#include <QByteArray>
#include <QHostAddress>
#include <QImage>
#include <QObject>
#include <QUdpSocket>
class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

public slots:
    Q_INVOKABLE void sendMessage(const QString &message);
    Q_INVOKABLE void sendImage(const QString &imagePath);
    Q_INVOKABLE void processPendingDatagrams();
    Q_INVOKABLE void startServer();
    Q_INVOKABLE void onMessageReceived(const QString &message);
    Q_INVOKABLE void onImageReceived(const QByteArray &imageData);
signals:
    void messageReceived(const QString &message);
    void imageReceived(const QByteArray &imageData);

private:
    QUdpSocket *udpSocket;
    // QMap<QString, QHostAddress>
    //clients; //restore estabilished client's ip Address and QHostAddress of object
};
