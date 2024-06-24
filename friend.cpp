#include "friend.h"

Friend::Friend(QObject *parent)
    : QObject{parent}
{
    m_name = "Alice";
    m_status = "online";
}
QString Friend::getName()
{
    return m_name;
}
QString Friend::getStatus()
{
    return m_status;
}
QString Friend::setName(QString name)
{
    m_name = name;
}
QString Friend::setStatus(QString status)
{
    m_status = status;
}
