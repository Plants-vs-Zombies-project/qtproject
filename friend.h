#pragma once

#include <QDateTime>
#include <QObject>
#include <QQmlEngine>
#include <QString>

class Friend : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit Friend(QObject *parent = nullptr);
    Q_INVOKABLE QString getName();
    Q_INVOKABLE QString getStatus();
    Q_INVOKABLE QString setName(QString name);
    Q_INVOKABLE QString setStatus(QString status);
    enum status { online, offline, away };
    Q_ENUM(status)

signals:
    void sendFile();
    void sendMsg();
    void statusChanged();

private:
    QString m_name;
    QString m_status;
};
