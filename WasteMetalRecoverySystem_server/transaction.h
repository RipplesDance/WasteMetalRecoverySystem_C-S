#ifndef TRANSACTION_H
#define TRANSACTION_H

#include<QtDebug>
#include<QDataStream>
#include<QString>
#include<QDateTime>
#include<QRandomGenerator>

class transaction
{
public:
    transaction(QString type = NULL);
    ~transaction();

    //set
    void setEnergyDensity(double energyDensity);
    void setWeight(double weight);
    void setSOH(double SOH);
    void setPrice(double price);
    void setLeagcyElectricity(double leagcyElectricity);
    void setTpye(QString type);
    void setUsagePurpose(QString usagePurpose);
    void setSellingWay(QString sellingWay);

    void setFilePath(QString filePath);
    void setUuid(QString uuid);
    void toogleAccept();
    void setResultTime(QDateTime resultTime);

    //quest
    QString getId();
    QString selectType();
    QString selectUsagePurpose();
    QString selectSellingWay();
    double selectEnergyDensity();
    double selectWeight();
    double selectSOH();
    double selectPrice();
    double selectLeagcyElectricity();
    bool checkStatus();
    QDateTime selectSubmittedTime();
    QDateTime selectResultTime();

    QString selectFilePath();
    QString getUuid();

protected:
    void setSubmittedTime(QDateTime submittedTime);
    void changeClassVersion(int version);

private:
    QString idGenerator();

    QString uuid;
    QString id;
    QString type;
    QString usagePurpose;
    QString sellingWay;
    double energyDensity;
    double weight;
    double SOH;
    double price;
    double leagcyElectricity;

    bool isAccepted;

    QDateTime submittedTime;
    QDateTime resultTime;

    int classVersion;

    QString filePath;

    friend QDataStream &operator<<(QDataStream &out, const transaction &data);
    friend QDataStream &operator>>(QDataStream &in, transaction &data);

};

//QDataStream &operator<<(QDataStream &out, const transaction &data);
// QDataStream &operator>>(QDataStream &in, transaction &data);

#endif // TRANSACTION_H
