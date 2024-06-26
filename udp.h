//本机之间发送信息和发送图片
#pragma once
#include <QObject>
//#include <QPixmap>
#include <QtNetwork/QUdpSocket>
class UDP : public QObject
{
    Q_OBJECT
public:
    explicit UDP(QObject *parent = nullptr);
    ~UDP();

    void sendMessage(const QString &message);
    void sendImage(const QByteArray &imageData);
    bool isJPEG(const char *data, int size);
    bool isPNG(const char *data, int size);
    bool isTextMessage(const char *data, int size);
signals:
    void messageReceived(const QString &message);
    void imageRecevied(const QByteArray &imageData);

private slots:
    //接受UDP数据
    void processPendingDatagrams();

private:
    int sockfd;
    void saveImage(const QString &filePath, const QByteArray &imageData);
};
