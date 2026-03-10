#ifndef QUOTATIONDIALOG_H
#define QUOTATIONDIALOG_H

#include <QDialog>
#include<QDebug>
#include<QListWidgetItem>
#include<QListWidget>
#include<QMessageBox>
#include"recoveryCost.h"

namespace Ui {
class quotationDialog;
}

class quotationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit quotationDialog(QWidget *parent = nullptr);
    ~quotationDialog();
    void addItemToListWidget(QString name);
    void setRecoveryCost(recoveryCost data);
    void setFinalPrice(double finalPrice);
    void clearListWidget();
    void focusFirstOnListWidget();

signals:
    void selected(QString key);
    void calculator(QString type, double energyDensity, double weight, double SOH, recoveryCost cost);
    void confirmed(QString key, recoveryCost value);

public slots:
    void itemClickedHandler(QListWidgetItem* item);
    void reset();
    void offFocus();
    void confirmChange();


public:
    //property
    recoveryCost originalCost;
private:
    Ui::quotationDialog *ui;
};

#endif // QUOTATIONDIALOG_H
