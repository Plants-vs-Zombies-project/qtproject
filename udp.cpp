//本机之间发送信息和发送图片
#include "udp.h"
#include <QDebug>
#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

UDP::UDP(QObject *parent)
    : QObject(this)
    , sockfd(-1)
{
    //创建udp套接字，如果调用失败返回-1
    //使用IPV4协议，UDP,系统自动选择协议
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        qDebug() << "Socket creation failed";
        return;
    }

    //设置addr为空
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    //初始化
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(
        INADDR_LOOPBACK); //设置本地回环地址127.0.0.1，htonl将主机字节序转换为大端字节序
    addr.sin_port = htons(12345);

    //将套接字绑定到&addr所初始化的ip和端口上
    if (bind(sockfd, (const struct sockaddr *) &addr, sizeof(addr)) < 0) {
        qDebug() << "bind failed!";
        close(sockfd);
        sockfd = -1;
        return;
    }

    //设置非阻塞模式
    fcntl(sockfd, F_SETFL, O_NONBLOCK);

    //连接接收数据信号
    //connect(this, &QUdpSocket::readyRead, this, &UDP::processPendingDatagrams);

    /*connect(this,
            &UDP::messageReceived,
            this,
            [](const QString &message, const QString &senderip, quint16 senderport) {
                //实现消息已读
                qDebug() << "收到消息：" << message;
                qDebug() << "来自：" << senderip << "端口：" << senderport;
            });
    qDebug() << "UDP socket created and bound";*/

    //保存收到的图片
    connect(this, &UDP::imageRecevied, this, &UDP::imageRecevied);
}

UDP::~UDP()
{
    if (sockfd != -1) {
        close(sockfd);
    }
}
//发送信息
void UDP::sendMessage(const QString &message)
{
    struct sockaddr_in addr;

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(12345);

    QByteArray datagram = message.toUtf8();
    //发送目标:addr;  datagram.constData()：要发送数据的缓冲区的指针 ；  datagram.size():要发送数据的长度
    ssize_t sent_bytes = sendto(sockfd,
                                datagram.constData(),
                                datagram.size(),
                                0,
                                (const struct sockaddr *) &addr,
                                sizeof(addr));
    if (sent_bytes == -1) {
        perror("Sendto failed");
    }
}
//发送图片
void UDP::sendImage(const QByteArray &imageData)
{
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    addr.sin_port = htons(12345);

    ssize_t send_bytess = sendto(sockfd,
                                 imageData.constData(),
                                 imageData.size(),
                                 0,
                                 (const struct sockaddr *) &addr,
                                 sizeof(addr));

    if (send_bytess == -1) {
        perror("Sendto failed");
    }
}

void UDP::processPendingDatagrams()
{
    struct sockaddr_in addr;
    socklen_t addrLen = sizeof(addr);
    char buffer[1024];

    /*接受信息
     * bytesRead:接收到的数据字节数
     * buffer：接受数据的缓冲区，数据将被复制到这里；
     * addr:接受发送者的地址信息
    */
    ssize_t bytesRead
        = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &addr, &addrLen);

    if (bytesRead == -1) {
        perror("Recvfrom failed");
        return;
    }
    //接受到图片信息
    if (isJPEG(buffer, bytesRead) || isPNG(buffer, bytesRead)) {
        QByteArray imageData(buffer, bytesRead);

        emit imageRecevied(imageData);
    } else if (isTextMessage(buffer, bytesRead)) { //如果成功接收，输出接收到的信息(文本）
        buffer[bytesRead] = '\0';                  //确保以'\0'结尾
        QString message = QString::fromUtf8(buffer);
        emit messageReceived(message);
        qDebug() << "Recevied message:" << message;
    }
}

bool UDP::isJPEG(const char *data, int size)
{
    const char *jpegMagicNumber = "\xFF\xD8\xFF";
    return size >= 3 && memcmp(data, jpegMagicNumber, 3) == 0;
}

bool UDP::isPNG(const char *data, int size)
{
    const char *pngMagicNumber = "x89\x50\x4E\x47\x0D\x1A\x0A";
    return size >= 8 && memcmp(data, pngMagicNumber, 0) == 0;
}

bool UDP::isTextMessage(const char *data, int size)
{
    return size > 0 && data[0] != '\0'; //假设文本不为空
}
