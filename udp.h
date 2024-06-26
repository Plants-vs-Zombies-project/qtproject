//本机之间发送信息和发送图片
#pragma once
#include <QObject>
#include <QPixmap>
#include <QtNetwork/QUdpSocket>
class UDP : public QObject
{
    Q_OBJECT
public:
    explicit UDP(QObject *parent = nullptr);
    ~UDP();

    void sendMessage(const QString &message);
    void sendImage(const QByteArray &imageData);

signals:
    void messageReceived(const QString &message, const QString &senderip, quint16 senderport);
    void imageRecevied();

public slots:
    void saveImage(const QPixmap &image, const QString &filePath);
private slots:
    void processPendingDatagrams();

private:
    int sockfd;
};
