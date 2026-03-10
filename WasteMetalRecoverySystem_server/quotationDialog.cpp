#include "quotationDialog.h"
#include "ui_quotationDialog.h"

quotationDialog::quotationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::quotationDialog)
{
    ui->setupUi(this);
    connect(ui->batteryName_listWidget,&QListWidget::itemClicked, this, &quotationDialog::itemClickedHandler);
    connect(ui->reset_btn,&QPushButton::clicked, this, &quotationDialog::reset);
    connect(ui->confirm_btn, &QPushButton::clicked,this, &quotationDialog::confirmChange);

    connect(ui->weight, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);
    connect(ui->SOH, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);
    connect(ui->energyDensity, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);

    connect(ui->unitPrice_80, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);
    connect(ui->unitPrice_90, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);
    connect(ui->price_per_kilo, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);
    connect(ui->profit, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);

    connect(ui->LCE_transitionRatio, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);
    connect(ui->CoSo4_transitionRatio, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);
    connect(ui->NiSo4_transitionRatio, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);
    connect(ui->MnSo4_transitionRatio, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &quotationDialog::offFocus);

    ui->usage_comboBox->addItem("金属回收");
    ui->usage_comboBox->addItem("电池梯次利用");
}

quotationDialog::~quotationDialog()
{
    delete ui;
}

void quotationDialog::clearListWidget()
{
    ui->batteryName_listWidget->clear();
}

void quotationDialog::confirmChange()
{
    if(!ui->batteryName_listWidget->currentItem()) return;
    if(ui->batteryName_listWidget->currentItem()->text().isEmpty())
        return;
    recoveryCost cost;
    cost.unitPrice_80 = ui->unitPrice_80->value();
    cost.unitPrice_90 = ui->unitPrice_90->value();
    cost.profit = ui->profit->value()/100;
    cost.price_per_kilo = ui->price_per_kilo->value();

    cost.LCE_transitionRatio = ui->LCE_transitionRatio->value()/100;
    cost.CoSo4_transitionRatio = ui->CoSo4_transitionRatio->value()/100;
    cost.NiSo4_transitionRatio = ui->NiSo4_transitionRatio->value()/100;
    cost.MnSo4_transitionRatio = ui->MnSo4_transitionRatio->value()/100;

    emit confirmed(ui->batteryName_listWidget->currentItem()->text(), cost);
    QMessageBox::information(this,"成功","更改报价参数成功!");
}

void quotationDialog::offFocus()
{
    if(!ui->batteryName_listWidget->currentItem()) return;
    if(ui->batteryName_listWidget->currentItem()->text().isEmpty()) return;

    qDebug()<<"quotationDialog::offFocus "<<ui->batteryName_listWidget->currentRow();
    double weight = ui->weight->value();
    if(weight == 0)
    {
        ui->finalPrice->setValue(0);
        return;
    }
    double energyDensity = ui->energyDensity->value();
    double SOH = ui->SOH->value()/100;
    if(SOH >= 0.8 && energyDensity > 0)
        ui->usage_comboBox->setCurrentText("电池梯次利用");
    else
        ui->usage_comboBox->setCurrentText("金属回收");

    recoveryCost cost;
    cost.unitPrice_80 = ui->unitPrice_80->value();
    cost.unitPrice_90 = ui->unitPrice_90->value();
    cost.profit = ui->profit->value()/100;
    cost.price_per_kilo = ui->price_per_kilo->value();

    cost.LCE_transitionRatio = ui->LCE_transitionRatio->value()/100;
    cost.CoSo4_transitionRatio = ui->CoSo4_transitionRatio->value()/100;
    cost.NiSo4_transitionRatio = ui->NiSo4_transitionRatio->value()/100;
    cost.MnSo4_transitionRatio = ui->MnSo4_transitionRatio->value()/100;

    qDebug()<<"quotationDialog::offFocus "<< ui->batteryName_listWidget->currentItem()->text() <<cost;
    emit calculator(ui->batteryName_listWidget->currentItem()->text(), energyDensity, weight, SOH, cost);
}

void quotationDialog::setFinalPrice(double finalPrice)
{
    ui->finalPrice->setValue(finalPrice);
}

void quotationDialog::focusFirstOnListWidget()
{
    ui->batteryName_listWidget->setCurrentRow(0);

    QString firstName = ui->batteryName_listWidget->currentItem()->text();
    emit selected(firstName);
}

void quotationDialog::setRecoveryCost(recoveryCost data)
{
    ui->unitPrice_80->setValue(data.unitPrice_80);
    ui->unitPrice_90->setValue(data.unitPrice_90);
    ui->price_per_kilo->setValue(data.price_per_kilo);
    ui->profit->setValue(data.profit*100);

    ui->LCE_transitionRatio->setValue(data.LCE_transitionRatio*100);
    ui->CoSo4_transitionRatio->setValue(data.CoSo4_transitionRatio*100);
    ui->NiSo4_transitionRatio->setValue(data.NiSo4_transitionRatio*100);
    ui->MnSo4_transitionRatio->setValue(data.MnSo4_transitionRatio*100);

    originalCost = data;
}

void quotationDialog::reset()
{
    ui->unitPrice_80->setValue(originalCost.unitPrice_80);
    ui->unitPrice_90->setValue(originalCost.unitPrice_90);
    ui->price_per_kilo->setValue(originalCost.price_per_kilo);
    ui->profit->setValue(originalCost.profit*100);

    ui->LCE_transitionRatio->setValue(originalCost.LCE_transitionRatio*100);
    ui->CoSo4_transitionRatio->setValue(originalCost.CoSo4_transitionRatio*100);
    ui->NiSo4_transitionRatio->setValue(originalCost.NiSo4_transitionRatio*100);
    ui->MnSo4_transitionRatio->setValue(originalCost.MnSo4_transitionRatio*100);
}

void quotationDialog::itemClickedHandler(QListWidgetItem* item)
{
    ui->batteryName_listWidget->setCurrentItem(item);
    QString name = item->text();
    emit selected(name);
}

void quotationDialog::addItemToListWidget(QString name)
{
    ui->batteryName_listWidget->addItem(name);
}
