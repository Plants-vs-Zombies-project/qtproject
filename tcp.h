#pragma once

#include <QDebug>
#include <QObject>
#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define BUF_SIZE 1024
class tcp : public QObject
{
    Q_OBJECT
public:
    explicit tcp(QObject *parent = nullptr);
    void C(QString file_path);
    void S(QString file_path);

private:
    int filetype(QString file_path);
    int switchfile(QString file_path);
    void foldertransmitC(QString file_path);
    void filetransmitC(QString file_path);
    void foldertransmitS(QString file_path);
    void filetransmitS(QString file_path);
    void switchflag(QString file_path);

    int _socketfd;
    sockaddr *_addr;
    socklen_t _len;
    int _socketfd2;
    int _flag;
};
