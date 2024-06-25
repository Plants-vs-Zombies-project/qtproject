#include "tcp.h"

tcp::tcp()
{
    _socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (!_socketfd) {
        qDebug << "socket erorr";
        perror("socket erorr");
        exit(1);
    }
    memset(&sockAddr, 0, sizeof(sockAddr));
    _addr.sin_family = PF_INET;
    _addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);
    if (bind(_socketfd, _addr, _len)) {
        qDebug << "bind erorr";
        perror("bind erorr");
        exit(1);
    }

    if (listen(_socketfd, 6)) {
        qDebug << "bind erorr";
        perror("bind erorr");
        exit(1);
    }

    _socketfd2 = accept(_socketfd, _addr, _len);

    recv(_socketfd2, _str, 255, 0);
}

int tcp::filetype(QString file_path)
{
    //QString buf;
    struct stat buf;
    int result;
    result = stat(file_path, &buf);
    if (result == 0) {
        qDebug << "stat error";
        exit(1);
    }
    if (S_IFDIR & buf.st_mode) {
        qDebug << "folder";
        return 1;
    } else if (S_IFREG & buf.st_mode) {
        qDebug << "file";
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
    pD = opendir(file_path);
    if (!pD) {
        qDebug << "opendir error";
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
    FILE* fp = fopen(file_path, "rb");
    if (fp == NULL) {
        qDebug << "Can't open file";
        exit(1);
    }
    int n = 1;
    while (n) {
        if (!feof(fp)) {
            memset(buf, 0, BUF_SIZE);
            fread(buf, 1, BUF_SIZE, fp);
            qDebug << "filetransmitC is reading file data";
            int i = send(_socketfd, buf, BUF_SIZE, 0);
            if (i == SOCKET_ERROR) {
                qDebug << "SOCKET_ERROR";
                exit(1);
            }

        } else {
            n = 0;
            break;
        }
    }
    std::cout << "filetransmitC  sucess" << std::endl;
    close(fp);
}

void tcp::foldertransmitS(QString file_path)
{
    mkdir(file_path, S_IRWXU || S_IRGRP || S_IROTH);
    DIR* pD;
    struct dirent* ptr;
    pD = opendir(file_path);
    if (!pD) {
        qDebug << "opendir error";
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
    FILE* fp = fopen(file_path, "w");
    int n = 1;
    while (n) {
        memset(buf, 0, BUF_SIZE);
        int i = recv(fp, buf, BUF_SIZE, 0);
        if (i == 0) {
            qDebug << "the network problem";
            n = 0;
            exit(1);
        }
        if (i == SOCKET_ERROR) {
            qDebug << "SOCKET_ERROR";
            n = 0;
            exit(1);
        }
        fwrite(buf, 1, BUF_SIZE, fp);
        qDebug << "filetransmits is storing file data";
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
