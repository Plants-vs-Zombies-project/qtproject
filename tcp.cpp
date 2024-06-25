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
    memset(&_addr, 0, sizeof(_addr));
    _addr->sa_family = PF_INET;
    // _addr->sa_addr.s_addr = inet_addr("127.0.0.1");
    // _addr.sin_port = htons(1234);
    if (bind(_socketfd, _addr, _len)) {
        std::cout << "bind erorr" << std::endl;
        perror("bind erorr");
        exit(1);
    }

    if (listen(_socketfd, 6)) {
        perror("bind erorr");
        exit(1);
    }

    _socketfd2 = accept(_socketfd, _addr, (socklen_t*) _len);
}

int tcp::filetype(QString file_path)
{
    struct stat buf;
    int result;
    char* ch;
    QByteArray ba = file_path.toLatin1();
    ch = ba.data();
    result = stat(ch, &buf);
    if (result == 0) {
        std::cout << "stat error" << std::endl;
        exit(1);
    }
    if (S_IFDIR & buf.st_mode) {
        std::cout << "folder" << std::endl;
        return 1;
    } else if (S_IFREG & buf.st_mode) {
        std::cout << "file" << std::endl;
        return 0;
    }
}

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
        std::cout << "file type error" << std::endl;
    }
}
//未完成自定义文件路径的传输
void tcp::foldertransmitC(QString file_path)
{
    DIR* pD;
    struct dirent* ptr;
    char* ch;
    QByteArray ba = file_path.toLatin1();
    ch = ba.data();
    pD = opendir(ch);
    if (!pD) {
        std::cout << "opendir error" << std::endl;
        exit(1);
    }
    while ((ptr = readdir(pD)) != 0) {
        if ((strcmp(ptr->d_name, ".")) && (strcmp(ptr->d_name, "..")))
            continue;
        filetransmitC(file_path + "/" + ptr->d_name);
    }
    std::cout << "foldertransmitC  sucess" << std::endl;
    return;
}

//未完成文件类型与文件名的传输
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
            // if (i == SOCKET_ERROR) {
            //     std::cout << "SOCKET_ERROR" << std::endl;
            //     exit(1);
            // }

        } else {
            n = 0;
            break;
        }
    }
    std::cout << "filetransmitC  sucess" << std::endl;
    //close(fp);
}

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
        std::cout << "opendir error" << std::endl;
        exit(1);
    }
    while ((ptr = readdir(pD)) != 0) {
        if ((strcmp(ptr->d_name, ".")) && (strcmp(ptr->d_name, "..")))
            continue;
        filetransmitS(file_path + "/" + ptr->d_name);
    }
    std::cout << "foldertransmitS  sucess" << std::endl;
    return;
}

//未完成文件类型与文件名的传输
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
            std::cout << "the network problem" << std::endl;
            n = 0;
            exit(1);
        }

        // if (i == SOCKET_ERROR) {
        //     std::cout << "SOCKET_ERROR" << std::endl;
        //     n = 0;
        //     exit(1);
        // }
        fwrite(buf, 1, BUF_SIZE, fp);
        std::cout << "filetransmits is storing file data" << std::endl;
    }
    std::cout << "filetransmitS  sucess" << std::endl;
}

void tcp::C(QString file_path)
{
    switchfile(file_path);
    if (_flag == 1) {
        foldertransmitC(file_path);
    }
    if (_flag == 2) {
        filetransmitC(file_path);
    }
}

void tcp::S(QString file_path)
{
    switchfile(file_path);
    if (_flag == 1) {
        foldertransmitS(file_path);
    }
    if (_flag == 2) {
        filetransmitS(file_path);
    }
}
