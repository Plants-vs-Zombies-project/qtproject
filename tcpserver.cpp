#include "tcpserver.h"
//初始化
TcpSeverer::TcpSeverer(QObject *parent)
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
    _addr->sin_port = htons(PORT2);
    //绑定
    if (bind(_socketfd, (struct sockaddr *) _addr, (socklen_t) _len)) {
        qDebug() << "bind error";
        exit(1);
    }
    //监听
    if (listen(_socketfd, MaxClient)) {
        qDebug() << "bind error";
        exit(1);
    }

    _socketfd2 = accept((int) _socketfd, (struct sockaddr *) addrs, (socklen_t *) sizeof(addrs));
}
//文件夹接收
void TcpSeverer::foldertransmitS(QString file_path)
{
    char *ch;
    QByteArray ba = file_path.toLatin1();
    ch = ba.data();
    mkdir(ch, S_IRWXU | S_IRGRP | S_IROTH);
    DIR *pD;
    struct dirent *ptr;
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
void TcpSeverer::filetransmitS(QString file_path)
{
    char buf[BUF_SIZE] = {0};
    char *ch;
    QByteArray ba = file_path.toLatin1();
    ch = ba.data();
    FILE *fp = fopen(ch, "w");
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
//判断文件类型
int TcpSeverer::filetype(QString file_path)
{
    struct stat buf;
    int result;
    char *ch;
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
// 置flag, 如文件夹则置1，文件则2
int TcpSeverer::switchfile(QString file_path)
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
    return 0;
}
//获得文件类型后决定使用哪个类型函数
void TcpSeverer::switchflag(QString file_path)
{
    //判断传输为文件还是文件夹
    switchfile(file_path);
    if (_flag == 1) {
        foldertransmitS(file_path);
    }
    if (_flag == 2) {
        filetransmitS(file_path);
    }
}
