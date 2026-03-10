#ifndef BATTERYMATERIALCONCENTRATION_H
#define BATTERYMATERIALCONCENTRATION_H

#include <QDebug>
#include<QDataStream>

class batteryMaterialConcentration
{

public:
    batteryMaterialConcentration(double li,double co,double mn,double ni,double cu,
                                 double compoundRatio,double positiveMaterialsRatio);
    batteryMaterialConcentration();
    ~batteryMaterialConcentration();

    void setRecycleRatio(double li_recycleRatio, double co_recycleRatio, double mn_recycleRatio,
                         double ni_recycleRatio, double cu_recycleRatio, double positiveMaterial_recycleRatio,
                         double compound_recycleRatio);


    friend QDataStream &operator<<(QDataStream &out, const batteryMaterialConcentration &data);
    friend QDataStream &operator>>(QDataStream &in, batteryMaterialConcentration &data);

    public:
    //material raito
    double li;
    double co;
    double mn;
    double ni;
    double cu;
    double compoundRatio;
    double positiveMaterialsRatio;

    //recycle ratio
    double li_recycleRatio;
    double co_recycleRatio;
    double mn_recycleRatio;
    double ni_recycleRatio;
    double cu_recycleRatio;
    double compound_recycleRatio;
    double positiveMaterial_recycleRatio;


};

#endif // BATTERYMATERIALCONCENTRATION_H
