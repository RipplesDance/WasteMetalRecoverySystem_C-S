#include "address.h"

address::address()
{
     fullName="";
     phoneNumber="";
     country="";
     province="";
     city="";
     district="";
     detail="";
     zipCode="";
    isDefault=false;
}
address::~address()
{

}

bool address::isValid() {
    return !fullName.isEmpty() && phoneNumber.length() >= 11 && !detail.isEmpty();
}

void address::createFilePath()
{
    QDir dir;
    if(!dir.exists(addressPath))
    {
        dir.mkdir(addressPath);
    }
}

void address::saveAddressToLocal(address data)
{
    createFilePath();
    QString fileName =data.id.toUtf8().toBase64();
    QFile file(addressPath + "/" + fileName + ".dat");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"无法打开文件"+fileName;
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out<< data;

    file.close();
}

QString address::removeFromLocal(const address data)
{
    QString fileName = data.id.toUtf8().toBase64();
    QString file = addressPath + "/" + fileName + ".dat";
    QFile local(file);
    if(!QFile::exists(file))
    {
        return "文件不存在";
    }
    if (local.remove())
        return "删除成功";
    else
        return "删除失败";
}

QList<address> address::readAllAddressFromLocal()
{
    createFilePath();
    QList<address> list;

    QDir dir(addressPath);
    QStringList filters;
    filters << "*.dat";
    dir.setNameFilters(filters);

    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);

    for (int i = 0; i < fileList.size(); ++i) {
        QFileInfo fileInfo = fileList.at(i);
        QFile file(fileInfo.absoluteFilePath());

        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_5_14);
            address obj;
            in >> obj;
            list.push_back(obj);

            file.close();
        }
    }
    return list;
}

QString address::generateId() {
    QString timePart = QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz");
    int randomPart = QRandomGenerator::global()->bounded(100, 999); // 3位随机数
    return QString("%1_%2").arg(timePart).arg(randomPart);
}

QString address::getFullAddress(const address* data = nullptr) {
    if(data != nullptr)
        return QString("%1 %2 %3 %4 %5")
               .arg(data->country).arg(data->province).arg(data->city).arg(data->district).arg(data->detail);
    return QString("%1 %2 %3 %4 %5")
           .arg(country).arg(province).arg(city).arg(district).arg(detail);
}

QDataStream &operator<<(QDataStream &out, const address &data)
{
    out<< data.id << data.fullName << data.phoneNumber<<data.country << data.province
      << data.city << data.district << data.detail << data.zipCode << data.isDefault;
    return out;
}

QDataStream &operator>>(QDataStream &in, address &data)
{
    in>> data.id >> data.fullName >> data.phoneNumber>>data.country >> data.province
      >> data.city >> data.district >> data.detail >> data.zipCode >> data.isDefault;
    return in;
}

QDebug operator<<(QDebug dbg, const address &obj)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "address("
                  << "id: " << obj.id << ", "
                  << "fullName: " << obj.fullName << ", "
                  << "phoneNumber: " << obj.phoneNumber << ", "
                  << "country: " << obj.country << ", "
                  << "province: " << obj.province << ", "
                  << "city: " << obj.city << ", "
                  << "district: " << obj.district << ", "
                  << "detail: " << obj.detail << ", "
                  << "zipCode: " << obj.zipCode << "，"
                  << "isDefault: " << obj.isDefault << ""
                  << ")";
        return dbg;
}
