#pragma once

#include <QDebug>
#include <QObject>
#include <QtQml/qqmlregistration.h>
#include <arpa/inet.h>
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
#define PORT2 8887
#define MaxClient 20

class TcpSeverer : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit TcpSeverer(QObject *parent = nullptr);
    //不同类型的文件传输
public:
    //判断传输文件的类型的函数
    Q_INVOKABLE void switchflag(QString file_path);
    //注意！qml中调用此函数即可接收文件夹或文件

private:
    Q_INVOKABLE void foldertransmitS(QString file_path); //文件夹类型接收
    Q_INVOKABLE void filetransmitS(QString file_path);   //文件类型接收
    //判断传输文件的类型的函数
    Q_INVOKABLE int switchfile(QString file_path);
    Q_INVOKABLE int filetype(QString file_path);

    int _socketfd;
    sockaddr_in *_addr;
    socklen_t _len;
    int _socketfd2;
    int _flag;
    sockaddr_in *addrs;
};
