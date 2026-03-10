#ifndef QUOTATION_H
#define QUOTATION_H

#include<QDebug>
#include<QMap>
#include<QMessageBox>
#include<QDataStream>
#include<QDir>
#include<QFile>
#include<QList>
#include<QDateTime>
#include"batteryMaterialConcentration.h"
#include"recoveryCost.h"
#include"metalPrice.h"

class quotation
{

public:
    quotation();
    ~quotation();
    double quotationCaculator(QString type, double energyDensity, double weight, double SOH);
    void init();
    void toogleTemporaryCostCalculator();
    void setTemporaryCost(recoveryCost data);

    void saveAllBatteryToLocal();
    void readAllBatteryFromLocal();
    void readAllRecoveryCostFromLocal();
    void setMetalPrice(metalPrice data);
    bool addBatteryType(QString key, batteryMaterialConcentration* data);
    bool addRecoveryCost(QString key, recoveryCost data);
    bool saveBatteryToLocal(QString key, batteryMaterialConcentration* data);
    bool saveRecoveryCostToLocal(QString key, recoveryCost data);
    bool removeBatteryFromLocal(QString key);
    bool removeRecoveryCostFromLocal(QString key);
    QList<QString> readAllBatteryType();
    QList<batteryMaterialConcentration*> readAllBatteryMaterialConcentration();
    QList<recoveryCost> readAllRecoveryCost();
    void changeBatteryValue(QString key, batteryMaterialConcentration* value);
    void changeRecoveryCostValue(QString key, recoveryCost value);
    bool changeBatteryNameKey(QString newKey, QString oldKey);
    bool changeRecoveryCostKey(QString newKey, QString oldKey);
    bool removeBatteryByName(QString key);
    bool removeRecoveryCostByName(QString key);
    bool renameLocalBattery(QString origin, QString name);
    bool renameLocalRecoveryCost(QString origin, QString name);
    void readMetalPriceFromLocal();
    void saveMetalPriceToLocal(metalPrice data);
    metalPrice getMetalPrice();
    recoveryCost fetchRecoveryCostByKey(QString key);
    batteryMaterialConcentration* fetchMaterialConcentrationByKey(QString key);

//    friend QDataStream &operator<<(QDataStream &out, const quotation &data);
//    friend QDataStream &operator>>(QDataStream &in, quotation &data);

private:
    QMap<QString, batteryMaterialConcentration*> batteryMap;
    QMap<QString, recoveryCost> recoveryCostMap;
    metalPrice metal_price;

public:

    //coefficient
    double Li_to_LCE;
    double Ni_to_NiSo4; // 6H₂O  NiSO₄·6H₂O
    double Co_to_CoSo4; // 7H₂O  CoSO₄·7H₂O
    double Mn_to_MnSo4;// 1H₂O MnSO₄·H₂O

private:
    //temporary property
    bool enableTemporaryCost;
    recoveryCost cost;
    batteryMaterialConcentration* battery;

    //file path
    QString recoveryCostPath = "bin/quotation_model/recoveryCost";
    QString batteryPath = "bin/quotation_model/battery";


};

#endif // QUOTATION_H
