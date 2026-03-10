#include "metalPrice.h"

metalPrice::metalPrice()
{
    liPrice = 0;
    mnPrice = 0;
    coPrice = 0;
    niPrice = 0;
    cuPrice = 0;
    isUpdated = false;
}

QDataStream &operator<<(QDataStream &out, const metalPrice &data)
{
    out << data.liPrice << data.coPrice << data.niPrice << data.mnPrice << data.cuPrice << data.isUpdated;
    return out;
}
QDataStream &operator>>(QDataStream &in, metalPrice &data)
{
    in >> data.liPrice >> data.coPrice >> data.niPrice >> data.mnPrice >> data.cuPrice >> data.isUpdated;
    return in;
}
QDebug operator<<(QDebug dbg, const metalPrice &obj)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "metalPrice("
                  << "liPrice: " << obj.liPrice << ", "
                  << "mnPrice: " << obj.mnPrice << ", "
                  << "coPrice: " << obj.coPrice << ", "
                  << "niPrice: " << obj.niPrice << ", "
                  << "cuPrice: " << obj.cuPrice << ", "
                  << "isUpdated: " << obj.isUpdated
                  << ")";
        return dbg;
}
