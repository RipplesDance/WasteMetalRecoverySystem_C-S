#include "recoveryCost.h"

recoveryCost::recoveryCost()
{
    this->unitPrice_80 = 0;
    this->unitPrice_90 = 0;

    this->price_per_kilo = 0;
    this->profit = 0;

    this->LCE_transitionRatio = 0;
    this->NiSo4_transitionRatio = 0;
    this->CoSo4_transitionRatio = 0;
    this->MnSo4_transitionRatio = 0;

    isUpdated= false;
}
recoveryCost::~recoveryCost(){}

void recoveryCost::setTransitionRatio(double LCE_transitionRatio,double CoSo4_transitionRatio,double NiSo4_transitionRatio,
                        double MnSo4_transitionRatio)
{
    this->LCE_transitionRatio = LCE_transitionRatio;
    this->NiSo4_transitionRatio = NiSo4_transitionRatio;
    this->CoSo4_transitionRatio = CoSo4_transitionRatio;
    this->MnSo4_transitionRatio = MnSo4_transitionRatio;
}

void recoveryCost::setProperty(double unitPrice_80, double unitPrice_90,
                               double price_per_kilo, double profit)
{
    this->unitPrice_80 = unitPrice_80;
    this->unitPrice_90 = unitPrice_90;

    this->price_per_kilo = price_per_kilo;
    this->profit = profit;
}

QDataStream &operator<<(QDataStream &out, const recoveryCost &data)
{
    out<<data.unitPrice_80<<data.unitPrice_90<<data.price_per_kilo<<data.profit << data.LCE_transitionRatio
      << data.CoSo4_transitionRatio << data.NiSo4_transitionRatio << data.MnSo4_transitionRatio << data.isUpdated;
    return out;
}

QDataStream &operator>>(QDataStream &in, recoveryCost &data)
{
    in >>data.unitPrice_80>>data.unitPrice_90>>data.price_per_kilo>>data.profit>> data.LCE_transitionRatio
       >> data.CoSo4_transitionRatio >> data.NiSo4_transitionRatio >> data.MnSo4_transitionRatio >> data.isUpdated;
    return in;
}

QDebug operator<<(QDebug dbg, const recoveryCost &obj)
{
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "recoveryCost("
                  << "unitPrice_80: " << obj.unitPrice_80 << ", "
                  << "unitPrice_90: " << obj.unitPrice_90 << ", "
                  << "price_per_kilo: " << obj.price_per_kilo << ", "
                  << "profit: " << obj.profit << ", "
                  << "LCE_transitionRatio: " << obj.LCE_transitionRatio << ", "
                  << "CoSo4_transitionRatio: " << obj.CoSo4_transitionRatio << ", "
                  << "NiSo4_transitionRatio: " << obj.NiSo4_transitionRatio << ", "
                  << "MnSo4_transitionRatio: " << obj.MnSo4_transitionRatio << ", "
                     << "MnSo4_transitionRatio: " << obj.isUpdated << ""
                  << ")";
        return dbg;
}
