#include "clientInfo.h"

clientInfo::clientInfo(QString uuid)
{
    loginTime = QDateTime::currentDateTime();
    this->uuid = uuid;
    heartBeatCount = 0;
    heartBeatTimer = new QTimer;
    connect(heartBeatTimer, &QTimer::timeout,this, [=](){
        heartBeatTimer->stop();
        emit zombie();
    });
}

clientInfo::~clientInfo()
{

}

void clientInfo::setIp(QString ip)
{
    this->ip = ip;
}

void clientInfo::heartBeat()
{
    heartBeatTimer->stop();
}

void clientInfo::testHeartBeat()
{
    heartBeatTimer->start(1000 * 15); //15 sec
}

QDateTime clientInfo::selectLoginTime(){return loginTime;}
QString clientInfo::getIp(){return ip;}
QString clientInfo::getUuid(){return uuid;}
