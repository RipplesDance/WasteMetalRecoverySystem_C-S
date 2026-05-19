#ifndef ADDRESS_H
#define ADDRESS_H

#include<QDebug>
#include<QDataStream>
#include<QList>
#include<QFile>
#include<QDir>
#include <QDateTime>
#include <QRandomGenerator>
#include<QFileInfo>
#include<QFileInfoList>

class address
{
public:
    address();
    ~address();
    QString getFullAddress(const address* data);
    bool isValid();
    void saveAddressToLocal(address data);
    QString removeFromLocal(const address data);
    QList<address> readAllAddressFromLocal();
    QString generateId();
    void createFilePath();

    friend QDataStream &operator<<(QDataStream &out, const address &data);
    friend QDataStream &operator>>(QDataStream &in, address &data);
    friend QDebug operator<<(QDebug dbg, const address &obj);
public:
    QString id;
    QString fullName;
    QString phoneNumber;
    QString country;
    QString province;
    QString city;
    QString district;
    QString detail;
    QString zipCode;
    bool isDefault;

    //dir path
    QString addressPath = "bin/address";
};

#endif // ADDRESS_H
