#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include<QDebug>
#include<QDateTime>
#include<QTimer>
#include<QObject>

class clientInfo : public QObject
{
    Q_OBJECT

public:
    clientInfo(QString uuid);
    ~clientInfo();
    void setIp(QString ip);

    //quest
    QDateTime selectLoginTime();
    QString getIp();
    QString getUuid();
    void testHeartBeat();
    void heartBeat();

    int heartBeatCount;
    QTimer* heartBeatTimer;

signals:
    void zombie();

private:
    QDateTime loginTime;
    QString ip;
    QString uuid;

};

#endif // CLIENTINFO_H
