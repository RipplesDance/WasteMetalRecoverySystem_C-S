#include "metalPriceDialog.h"
#include "ui_metalPriceDialog.h"

metalPriceDialog::metalPriceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::metalPriceDialog)
{
    ui->setupUi(this);

    init();
    connect(ui->update_btn, &QPushButton::clicked, [=](){
        double li_price = ui->liPrice_spinBox->value();
        double co_price = ui->coPrice_spinBox->value();
        double ni_price = ui->niPrice_spinBox->value();
        double mn_price = ui->mnPrice_spinBox->value();
        double cu_price = ui->cuPrice_spinBox->value();

        metalPrice data;
        data.liPrice = li_price;
        data.coPrice = co_price;
        data.niPrice = ni_price;
        data.mnPrice = mn_price;
        data.cuPrice = cu_price;
        data.isUpdated = true;

        emit update(data);
        init();
    });
    connect(ui->clear_btn, &QPushButton::clicked, this, &metalPriceDialog::init);
}

metalPriceDialog::~metalPriceDialog()
{
    delete ui;
}

void metalPriceDialog::init()
{
    ui->liPrice_spinBox->setValue(0);
    ui->coPrice_spinBox->setValue(0);
    ui->niPrice_spinBox->setValue(0);
    ui->mnPrice_spinBox->setValue(0);
    ui->cuPrice_spinBox->setValue(0);
}

void metalPriceDialog::setMetalPrice(metalPrice data)
{
    ui->liPrice_spinBox->setValue(data.liPrice);
    ui->coPrice_spinBox->setValue(data.coPrice);
    ui->niPrice_spinBox->setValue(data.niPrice);
    ui->mnPrice_spinBox->setValue(data.mnPrice);
    ui->cuPrice_spinBox->setValue(data.cuPrice);
}
