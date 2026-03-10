#ifndef CLIENTSETTING_H
#define CLIENTSETTING_H

#include<QDebug>
#include<QDataStream>

class clientSetting
{
public:
    clientSetting();

    friend QDataStream &operator<<(QDataStream &out, const clientSetting &data);
    friend QDataStream &operator>>(QDataStream &in, clientSetting &data);
    friend QDebug operator<<(QDebug dbg, const clientSetting &obj);

public:
    QString ip;
    quint16 port;
    int width;
    int height;
    QString transactionPath;
    int waittingTime;
};

#endif // CLIENTSETTING_H
