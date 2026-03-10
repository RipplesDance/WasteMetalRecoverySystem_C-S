#include "batteryMaterialConcentration.h"

batteryMaterialConcentration::batteryMaterialConcentration(double li,double co,double mn,double ni,double cu,
                                                           double compoundRatio,double positiveMaterialsRatio)
    :li(li),co(co),mn(mn),ni(ni),cu(cu), compoundRatio(compoundRatio), positiveMaterialsRatio(positiveMaterialsRatio)
{
}

batteryMaterialConcentration::batteryMaterialConcentration(){
     li=0;
     co=0;
     mn=0;
     ni=0;
     cu=0;
     compoundRatio=0;
     positiveMaterialsRatio=0;

     li_recycleRatio=0;
     co_recycleRatio=0;
     mn_recycleRatio=0;
     ni_recycleRatio=0;
     cu_recycleRatio=0;
     compound_recycleRatio=0;
     positiveMaterial_recycleRatio=0;
}
batteryMaterialConcentration::~batteryMaterialConcentration()
{}

void batteryMaterialConcentration::setRecycleRatio(double li_recycleRatio, double co_recycleRatio,
                                                   double mn_recycleRatio, double ni_recycleRatio,
                                                   double cu_recycleRatio, double positiveMaterial_recycleRatio,
                                                   double compound_recycleRatio)
{
    this->li_recycleRatio = li_recycleRatio;
    this->co_recycleRatio = co_recycleRatio;
    this->mn_recycleRatio = mn_recycleRatio;
    this->ni_recycleRatio = ni_recycleRatio;
    this->cu_recycleRatio = cu_recycleRatio;
    this->positiveMaterial_recycleRatio = positiveMaterial_recycleRatio;
    this->compound_recycleRatio = compound_recycleRatio;
}

QDataStream &operator<<(QDataStream &out, const batteryMaterialConcentration &data)
{
    out << data.li << data.co << data.ni << data.mn << data.cu << data.compoundRatio << data.positiveMaterialsRatio
        << data.li_recycleRatio << data.co_recycleRatio << data.ni_recycleRatio<< data.mn_recycleRatio
        << data.cu_recycleRatio << data.positiveMaterial_recycleRatio << data.compound_recycleRatio;
    return out;
}
QDataStream &operator>>(QDataStream &in, batteryMaterialConcentration &data)
{
    in >> data.li >> data.co >> data.ni >> data.mn >> data.cu >> data.compoundRatio >> data.positiveMaterialsRatio
        >> data.li_recycleRatio >> data.co_recycleRatio >> data.ni_recycleRatio>> data.mn_recycleRatio >>
            data.cu_recycleRatio >> data.positiveMaterial_recycleRatio >> data.compound_recycleRatio;
    return in;
}
