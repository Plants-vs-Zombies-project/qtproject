#include "tcpclient.h"

tcpClient::tcpClient(QString file_path, QObject *parent = nullptr)
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

    ::connect(_socketfd, (struct sockaddr *) addrs, sizeof(addrs));
    switchfile(file_path);
    if (_flag == 1) {
        foldertransmitC(file_path);
    }
    if (_flag == 2) {
        filetransmitC(file_path);
    }
}

void tcpClient::foldertransmitC(QString file_path)
{
    DIR *pD;
    struct dirent *ptr;
    char *ch;
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

void tcpClient::filetransmitC(QString file_path)
{
    char buf[BUF_SIZE] = {0};
    char *ch;
    QByteArray ba = file_path.toLatin1();
    ch = ba.data();
    FILE *fp = fopen(ch, "rb");
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

int tcpClient::filetype(QString file_path)
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

int tcpClient::switchfile(QString file_path)
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

void tcpClient::switchflag(QString file_path) {}
