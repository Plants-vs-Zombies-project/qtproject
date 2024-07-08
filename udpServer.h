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
    void sendMessage(const QString &message);
    void sendImage(const QString &imagePath);
    void processPendingDatagrams();
    void startServer();
    void onMessageReceived(const QString &message);
    void onImageReceived(const QByteArray &imageData);
signals:
    void messageReceived(const QString &message);
    void imageReceived(const QByteArray &imageData);

private:
    QUdpSocket *udpSocket;
    // QMap<QString, QHostAddress>
    //clients; //restore estabilished client's ip Address and QHostAddress of object
};
