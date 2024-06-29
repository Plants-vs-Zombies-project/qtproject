#include "tcp.h"

tcp::tcp(QObject* parent)
    : QObject(parent)
{
    _socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!_socketfd) {
        std::cout << "socket erorr" << std::endl;
        perror("socket erorr");
        exit(1);
    }
    memset(&addrs, 0, sizeof(addrs));
    memset(&_addr, 0, sizeof(_addr));
    _addr->sin_family = PF_INET;
    _addr->sin_addr.s_addr = inet_addr("127.0.0.1");
    _addr->sin_port = htons(PORT);
}
//判断传输的是文件还是目录
int tcp::filetype(QString file_path)
{
    struct stat buf;
    int result;
    char* ch;
    QByteArray ba = file_path.toLatin1();
    ch = ba.data();
    result = stat(ch, &buf);
    if (result == 0) {
        qDebug() << "stat error";
        exit(1);
    }
    if (S_IFDIR & buf.st_mode) {
        qDebug() << "folder";
        return 1;
    } else if (S_IFREG & buf.st_mode) {
        qDebug() << "file";
        return 0;
    }
    return 1;
}
//
int tcp::switchfile(QString file_path)
{
    int file_type;
    file_type = filetype(file_path);
    switch (file_type) {
    case 1:
        _flag = 1;
        break;
    case 0:
        _flag = 2;
        break;
    default:
        qDebug() << "file type error";
    }
}
//文件目录发送
void tcp::foldertransmitC(QString file_path)
{
    DIR* pD;
    struct dirent* ptr;
    char* ch;
    QByteArray ba = file_path.toLatin1();
    ch = ba.data();
    pD = opendir(ch);
    if (!pD) {
        qDebug() << "opendir error";
        exit(1);
    }
    while ((ptr = readdir(pD)) != 0) {
        if ((strcmp(ptr->d_name, ".")) && (strcmp(ptr->d_name, "..")))
            continue;
        filetransmitC(file_path + "/" + ptr->d_name);
    }
    qDebug() << "foldertransmitC  sucess";
    return;
}

//文件发送
void tcp::filetransmitC(QString file_path)
{
    char buf[BUF_SIZE] = {0};
    char* ch;
    QByteArray ba = file_path.toLatin1();
    ch = ba.data();
    FILE* fp = fopen(ch, "rb");
    if (fp == NULL) {
        std::cout << "Can't open file" << std::endl;
        exit(1);
    }
    int n = 1;
    while (n) {
        if (!feof(fp)) {
            memset(buf, 0, BUF_SIZE);
            fread(buf, 1, BUF_SIZE, fp);
            std::cout << "filetransmitC is reading file data" << std::endl;
            int i = send(_socketfd, buf, BUF_SIZE, 0);

        } else {
            n = 0;
            break;
        }
    }
    qDebug() << "filetransmitC  sucess";
}
//文件目录接收
void tcp::foldertransmitS(QString file_path)
{
    char* ch;
    QByteArray ba = file_path.toLatin1();
    ch = ba.data();
    mkdir(ch, S_IRWXU | S_IRGRP | S_IROTH);
    DIR* pD;
    struct dirent* ptr;
    pD = opendir(ch);
    if (!pD) {
        qDebug() << "opendir error";
        exit(1);
    }
    while ((ptr = readdir(pD)) != 0) {
        if ((strcmp(ptr->d_name, ".")) && (strcmp(ptr->d_name, "..")))
            continue;
        filetransmitS(file_path + "/" + ptr->d_name);
    }
    qDebug() << "foldertransmitS  sucess";
    return;
}

//文件接收
void tcp::filetransmitS(QString file_path)
{
    char buf[BUF_SIZE] = {0};
    char* ch;
    QByteArray ba = file_path.toLatin1();
    ch = ba.data();
    FILE* fp = fopen(ch, "w");
    int n = 1;
    while (n) {
        memset(buf, 0, BUF_SIZE);
        int i = recv(_socketfd, buf, BUF_SIZE, 0);
        std::fstream f;
        f.open(ch, std::ios::in);
        f << buf;
        if (i == 0) {
            qDebug() << "the network problem";
            n = 0;
            exit(1);
        }

        fwrite(buf, 1, BUF_SIZE, fp);
        qDebug() << "filetransmits is storing file data";
    }
    qDebug() << "filetransmitS  sucess";
}
//客户端发送
void tcp::C(QString file_path)
{
    ::connect(_socketfd, (struct sockaddr*) addrs, sizeof(addrs));
    switchfile(file_path);
    if (_flag == 1) {
        foldertransmitC(file_path);
    }
    if (_flag == 2) {
        filetransmitC(file_path);
    }
}
//服务端接收
void tcp::S(QString file_path)
{
    if (bind(_socketfd, (struct sockaddr*) _addr, (socklen_t) _len)) {
        qDebug() << "bind error";
        exit(1);
    }

    if (listen(_socketfd, MaxClient)) {
        qDebug() << "bind error";
        exit(1);
    }

    _socketfd2 = accept((int) _socketfd, (struct sockaddr*) addrs, (socklen_t*) sizeof(addrs));
    switchfile(file_path);
    if (_flag == 1) {
        foldertransmitS(file_path);
    }
    if (_flag == 2) {
        filetransmitS(file_path);
    }
}
