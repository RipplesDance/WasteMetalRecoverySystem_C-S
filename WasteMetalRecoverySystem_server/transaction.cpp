#include "transaction.h"

transaction::transaction(QString type):type(type)
{
    isAccepted = false;
    changeClassVersion(1);
    setSubmittedTime(QDateTime::currentDateTime());
    id = idGenerator();
}
transaction::~transaction()
{

}

QString transaction::idGenerator()
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    int random = QRandomGenerator::global()->bounded(1000, 10000);
    return timestamp + QString::number(random);
}

void transaction::setEnergyDensity(double energyDensity)
{
    this->energyDensity = energyDensity;
}

void transaction::setWeight(double weight)
{
    this->weight = weight;
}

void transaction::setSOH(double SOH)
{
    this->SOH = SOH;
}

void transaction::setPrice(double price)
{
    this->price = price;
}

void transaction::setLeagcyElectricity(double leagcyElectricity)
{
    this->leagcyElectricity = leagcyElectricity;
}

void transaction::setTpye(QString type)
{
    this->type = type;
}

void transaction::setFilePath(QString filePath)
{
    this->filePath = filePath;
}

void transaction::setSellingWay(QString sellingWay)
{
    this->sellingWay = sellingWay;
}

void transaction::toogleAccept()
{
    isAccepted = !isAccepted;
}

void transaction::setUsagePurpose(QString usagePurpose)
{
    this->usagePurpose = usagePurpose;
}

void transaction::setSubmittedTime(QDateTime submittedTime)
{
    this->submittedTime = submittedTime;
}

void transaction::setResultTime(QDateTime resultTime)
{
    this->resultTime = resultTime;
}

void transaction::changeClassVersion(int version)
{
    this->classVersion = version;
}

void transaction::setUuid(QString uuid)
{
    this->uuid = uuid;
}

 QDataStream &operator<<(QDataStream &out, const transaction &data)
 {

    out << data.classVersion << data.id << data.type << data.energyDensity << data.weight << data.SOH << data.price
        << data.usagePurpose << data.leagcyElectricity << data.sellingWay << data.isAccepted
        << data.submittedTime << data.resultTime << data.filePath << data.uuid;
    return out;
 }

QDataStream &operator>>(QDataStream &in, transaction &data)
{

    in >> data.classVersion;
    if(data.classVersion == 1)
    {
        in >> data.id >> data.type >> data.energyDensity >> data.weight >> data.SOH >> data.price
                >> data.usagePurpose >> data.leagcyElectricity >> data.sellingWay >> data.isAccepted
                >> data.submittedTime >> data.resultTime >> data.filePath >> data.uuid;
    }

    return in;
}

QString transaction::getId(){return id;}
QString transaction::selectType(){return type;}
QString transaction::selectUsagePurpose(){return usagePurpose;}
QString transaction::selectSellingWay(){return sellingWay;}
double transaction::selectEnergyDensity(){return energyDensity;}
double transaction::selectWeight(){return weight;}
double transaction::selectSOH(){return SOH;}
double transaction::selectPrice(){return price;}
double transaction::selectLeagcyElectricity(){return leagcyElectricity;}
bool transaction::checkStatus(){return isAccepted;}
QDateTime transaction::selectSubmittedTime(){return submittedTime;}
QDateTime transaction::selectResultTime(){return resultTime;}
QString transaction::selectFilePath(){return filePath;}
QString transaction::getUuid(){return uuid;}
