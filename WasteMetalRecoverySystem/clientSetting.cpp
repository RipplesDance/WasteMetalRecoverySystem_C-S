#include "clientSetting.h"

clientSetting::clientSetting()
{
    ip = "127.0.0.1";
    port = 8888;
     width = 1320;
     height = 820;
     transactionPath = "bin/transactions";
     waittingTime = 5;
}


QDataStream &operator<<(QDataStream &out, const clientSetting &data)
{
    out<<data.ip<<data.port<<data.width<<data.height<<data.transactionPath<<data.waittingTime;
    return out;
}
QDataStream &operator>>(QDataStream &in, clientSetting &data)
{
    in>>data.ip>>data.port>>data.width>>data.height>>data.transactionPath>>data.waittingTime;
    return in;
}
QDebug operator<<(QDebug dbg, const clientSetting &obj)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "clientSetting("
                  << "ip: " << obj.ip << ", "
                  << "port: " << obj.port << ", "
                  << "width: " << obj.width << ", "
                  << "height: " << obj.height << ", "
                  << "transactionPath: " << obj.transactionPath << ", "
                  << "waittingTime: " << obj.waittingTime
                  << ")";
        return dbg;
}
