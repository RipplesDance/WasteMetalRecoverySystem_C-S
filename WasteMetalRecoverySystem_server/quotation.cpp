#include "quotation.h"

quotation::quotation()
{
    init();

    if(batteryMap.isEmpty())
    {
        batteryMaterialConcentration* LCO = new batteryMaterialConcentration(0.071,0.602,0,0,0.1, 0.93, 0.35);
        LCO->setRecycleRatio(0.85, 0.95,0,0,0.98,0.95,0.95);
        batteryMap.insert("钴酸锂电池", LCO);

        recoveryCost cost;
        cost.setProperty(0.3, 0.35,45,0.2);
        cost.isUpdated = true;
        cost.LCE_transitionRatio = 0.85;
        cost.CoSo4_transitionRatio = 0.85;
        recoveryCostMap.insert("钴酸锂电池", cost);

        saveBatteryToLocal("钴酸锂电池", LCO);
        saveRecoveryCostToLocal("钴酸锂电池", cost);
    }
    if(!metal_price.isUpdated)
    {
        metal_price.liPrice = 136038;
        metal_price.mnPrice = 6940;
        metal_price.coPrice = 96664;
        metal_price.niPrice = 31645;
        metal_price.cuPrice = 99605;
        metal_price.isUpdated = true;
        saveMetalPriceToLocal(metal_price);
    }

}

quotation:: ~quotation()
{
    qDeleteAll(batteryMap);
    batteryMap.clear();
}

void quotation::toogleTemporaryCostCalculator()
{
    enableTemporaryCost = !enableTemporaryCost;
}

void quotation::setTemporaryCost(recoveryCost data)
{
    this->cost = data;
}

void quotation::init()
{

    Li_to_LCE = 73.89/ (6.94*2); // Li₂CO₃ = 73.89, Li = 6.94
    Ni_to_NiSo4 = 262.87/ 58.69; // 6H₂O  NiSO₄·6H₂O = 262.87, Ni = 58.69
    Co_to_CoSo4 = 281.10/ 58.93; // 7H₂O  CoSO₄·7H₂O = 281.10, Co = 58.93
    Mn_to_MnSo4 = 169.02/ 54.94;// 1H₂O MnSO₄·H₂O = 169.02, Mn = 54.94

    enableTemporaryCost = false;


    //check quotation model directory
    QDir dir;
    //check cost directory
    if (!dir.exists(recoveryCostPath)) dir.mkpath(recoveryCostPath);

    //check battery directory
    if (!dir.exists(batteryPath)) dir.mkpath(batteryPath);

    readMetalPriceFromLocal();
    readAllBatteryFromLocal();
    readAllRecoveryCostFromLocal();
}

//other functions
void quotation::changeBatteryValue(QString key, batteryMaterialConcentration* value)
{
    if (value == nullptr) return;

    if (batteryMap.contains(key)) {
        // exist
        batteryMaterialConcentration *oldData = batteryMap.take(key);
        if(oldData != value)
            delete oldData;
    }
    batteryMap.insert(key,value);
}

void quotation::changeRecoveryCostValue(QString key, recoveryCost value)
{
    recoveryCostMap.insert(key,value);
}

bool quotation::changeBatteryNameKey(QString newKey, QString oldKey)
{
    if(!batteryMap.contains(oldKey)) return false;
    if(batteryMap.contains(newKey)) return false;

    batteryMaterialConcentration* data = batteryMap.take(oldKey);
    batteryMap.insert(newKey, data);

    return  true;
}

bool quotation::changeRecoveryCostKey(QString newKey, QString oldKey)
{
    if(!recoveryCostMap.contains(oldKey)) return false;
    if(recoveryCostMap.contains(newKey)) return false;

    recoveryCostMap.insert(newKey, fetchRecoveryCostByKey(oldKey));
    recoveryCostMap.remove(oldKey);

    return  true;
}

bool quotation::removeBatteryByName(QString key)
{
    if (!batteryMap.contains(key)) {
        return false;
    }
    delete batteryMap.take(key); //delete original value
    return true;
}

bool quotation::removeRecoveryCostByName(QString key)
{
    if (!recoveryCostMap.contains(key)) {
        return false;
    }
    recoveryCostMap.remove(key);
    return true;
}

bool quotation::renameLocalBattery(QString origin, QString name)
{
    QString oldPath = batteryPath + "/" + origin.toUtf8().toBase64() + ".dat";
    QString newPath = batteryPath + "/" + name.toUtf8().toBase64() + ".dat";

    if (!QFile::exists(oldPath))
        return false;

    QFile::rename(oldPath, newPath);
    return true;
}

bool quotation::renameLocalRecoveryCost(QString origin, QString name)
{
    QString oldPath = recoveryCostPath + "/" + origin.toUtf8().toBase64() + ".dat";
    QString newPath = recoveryCostPath + "/" + name.toUtf8().toBase64() + ".dat";

    if (!QFile::exists(oldPath))
        return false;

    QFile::rename(oldPath, newPath);
    return true;
}

QList<QString> quotation::readAllBatteryType()
{
    return batteryMap.keys();
}

QList<batteryMaterialConcentration*> quotation::readAllBatteryMaterialConcentration()
{
    return batteryMap.values();
}

QList<recoveryCost> quotation::readAllRecoveryCost()
{
    return recoveryCostMap.values();
}

bool quotation::saveBatteryToLocal(QString key, batteryMaterialConcentration* data)
{
    if (data == nullptr) return false;
    QString fileName =key.toUtf8().toBase64();

    QFile file(batteryPath + "/" + fileName + ".dat");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"无法打开文件"+fileName;
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out<<*(data);
    return true;
}

bool quotation::saveRecoveryCostToLocal(QString key, recoveryCost data)
{
    QString fileName =key.toUtf8().toBase64();

    QFile file(recoveryCostPath + "/" + fileName + ".dat");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"无法打开文件"+fileName;
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out<< data;
    return true;
}

bool quotation::removeBatteryFromLocal(QString key)
{
    QString fileName =key.toUtf8().toBase64();

    QString filePath = batteryPath + "/" + fileName + ".dat";
    QFile file(filePath);
    if(!QFile::exists(filePath))
    {
        qDebug()<<"文件不存在";
        return false;
    }
    if (file.remove())
        return true;
    else
        return false;
}

bool quotation::removeRecoveryCostFromLocal(QString key)
{
    QString fileName =key.toUtf8().toBase64();

    QString filePath = recoveryCostPath + "/" + fileName + ".dat";
    QFile file(filePath);
    if(!QFile::exists(filePath))
    {
        qDebug()<<"文件不存在";
        return false;
    }
    if (file.remove())
        return true;
    else
        return false;
}

bool quotation::addBatteryType(QString key, batteryMaterialConcentration* data)
{
    if(!data) return false;
    if(batteryMap.contains(key))
        return false;
    batteryMap.insert(key, data);
    return true;
}

bool quotation::addRecoveryCost(QString key, recoveryCost data)
{
    if(recoveryCostMap.contains(key)) return false;
    recoveryCostMap.insert(key, data);
    return true;
}

void quotation::saveAllBatteryToLocal()
{
    QMap<QString, batteryMaterialConcentration*>::iterator it = batteryMap.begin();
    while(it != batteryMap.end())
    {
        QString fileName =it.key().toUtf8().toBase64();

        QFile file(batteryPath + "/" + fileName + ".dat");
        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug()<<"无法打开文件保存batteryMaterialConcentration";
            return;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_14);
        out<<*(it.value());
        it++;
    }
}

void quotation::readAllBatteryFromLocal()
{
    qDeleteAll(batteryMap);
    batteryMap.clear();

    QDir dir(batteryPath);
    QStringList filters;
    filters << "*.dat";
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QFile file(fileInfo.absoluteFilePath());

        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_5_14);

            QString fileName = fileInfo.baseName();
            QByteArray base64Data = QByteArray::fromBase64(fileName.toUtf8());
            QString key = QString::fromUtf8(base64Data);

            batteryMaterialConcentration *obj = new batteryMaterialConcentration();
            in >> *obj;

            batteryMap.insert(key, obj);

            file.close();
        }
    }
}

void quotation::readAllRecoveryCostFromLocal()
{
    recoveryCostMap.clear();

    QDir dir(recoveryCostPath);
    QStringList filters;
    filters << "*.dat";
    dir.setNameFilters(filters);

    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);

    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QFile file(fileInfo.absoluteFilePath());

        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_5_14);

            QString fileName = fileInfo.baseName();
            QByteArray base64Data = QByteArray::fromBase64(fileName.toUtf8());
            QString key = QString::fromUtf8(base64Data);

            recoveryCost obj;
            in >> obj;

            recoveryCostMap.insert(key, obj);

            file.close();
        }
    }
}

void quotation::saveMetalPriceToLocal(metalPrice data)
{
    QFile file("bin/quotation_model/metalPrice_CNY.dat");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug("无法保存本地金属价格信息!");
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out << data;
}

void quotation::readMetalPriceFromLocal()
{
    QFile file("bin/quotation_model/metalPrice_CNY.dat");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug("无法读取本地金属价格信息!");
        return;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_14);
    in >> metal_price;
}

void quotation::setMetalPrice(metalPrice data)
{
    this->metal_price = data;
}

metalPrice quotation::getMetalPrice(){return metal_price;}

recoveryCost quotation::fetchRecoveryCostByKey(QString key)
{
    return recoveryCostMap.value(key);
}

batteryMaterialConcentration* quotation::fetchMaterialConcentrationByKey(QString key)
{
    if(batteryMap.contains(key))
        return batteryMap.value(key);
    return nullptr;
}

//core model functions
double quotation::quotationCaculator(QString type, double energyDensity, double weight, double SOH)
{
    if(!batteryMap.contains(type))
    {
        QMessageBox::information(nullptr,"提示", "当前暂不支持该电池的报价");
        return 0.0;
    }

    recoveryCost cost = recoveryCostMap.value(type);
    batteryMaterialConcentration* battery = batteryMap.value(type);
    //check if enable temporary cost calculator
    if(enableTemporaryCost)
    {
        cost = this->cost;
    }

    double recyclingPrice = 0;
    if(SOH >= 0.8 && energyDensity > 0)
    {
        //reusable
        recyclingPrice = weight * energyDensity * SOH;
        if(SOH>=0.9)
            recyclingPrice *= cost.unitPrice_90;
        else
            recyclingPrice *= cost.unitPrice_80;
    }

    double positiveMaterial = weight * battery->positiveMaterialsRatio * battery->positiveMaterial_recycleRatio;
    double positiveMaterialCompound = positiveMaterial * battery->compoundRatio * battery->compound_recycleRatio;

    //metal amount
    double li_amount =  positiveMaterialCompound * battery->li * battery->li_recycleRatio;
    double co_amount = positiveMaterialCompound * battery->co * battery->co_recycleRatio;
    double mn_amount = positiveMaterialCompound * battery->mn * battery->mn_recycleRatio;
    double ni_amount = positiveMaterialCompound * battery->ni * battery->ni_recycleRatio;

    //metal price
    double li_quotation = li_amount * Li_to_LCE * (metal_price.liPrice/1000) * cost.LCE_transitionRatio;
    double ni_quotation = ni_amount * Ni_to_NiSo4 * (metal_price.niPrice/1000) * cost.NiSo4_transitionRatio;
    double co_quotation = co_amount * Co_to_CoSo4 * (metal_price.coPrice/1000) * cost.CoSo4_transitionRatio;
    double mn_quotation = mn_amount * Mn_to_MnSo4 * (metal_price.mnPrice/1000) * cost.MnSo4_transitionRatio;

    double cu_quotation = weight * battery->cu * battery->cu_recycleRatio * (metal_price.cuPrice/1000);

    //final price
    double finalPrice = (li_quotation+ co_quotation+ mn_quotation+ ni_quotation+ cu_quotation)
            - cost.price_per_kilo * weight;

    return finalPrice > recyclingPrice ? finalPrice* (1 - cost.profit) : recyclingPrice;

}

//QDataStream &operator<<(QDataStream &out, const quotation &data)
//{
//    out<<data.LCE_transitionRatio << data.CoSo4_transitionRatio << data.NiSo4_transitionRatio << data.MnSo4_transitionRatio;
//    return out;
//}

//QDataStream &operator>>(QDataStream &in, quotation &data)
//{
//    in>>data.LCE_transitionRatio >> data.CoSo4_transitionRatio >> data.NiSo4_transitionRatio >> data.MnSo4_transitionRatio;
//    return in;
//}
