#ifndef RECOVERYCOST_H
#define RECOVERYCOST_H

#include<QDebug>
#include<QDataStream>

class recoveryCost
{
public:
    recoveryCost();
    ~recoveryCost();

    void setProperty(double unitPrice_80, double unitPrice_90, double price_per_kilo, double profit);
    void setTransitionRatio(double LCE_transitionRatio,double NiSo4_transitionRatio,double CoSo4_transitionRatio,
                            double MnSo4_transitionRatio);

    friend QDataStream &operator<<(QDataStream &out, const recoveryCost &data);
    friend QDataStream &operator>>(QDataStream &in, recoveryCost &data);
    friend QDebug operator<<(QDebug dbg, const recoveryCost &obj);

public:
    //property
    double unitPrice_80;//CNY/Wh. Depends on market
    double unitPrice_90;//CNY/Wh. Depends on market

    double price_per_kilo; //include electricity fee， labor fee, and chemicals fee. Count in CNY.
    double profit;
    //transition efficiency
    double LCE_transitionRatio;
    double NiSo4_transitionRatio;
    double CoSo4_transitionRatio;
    double MnSo4_transitionRatio;

    bool isUpdated;
};

#endif // RECOVERYCOST_H
