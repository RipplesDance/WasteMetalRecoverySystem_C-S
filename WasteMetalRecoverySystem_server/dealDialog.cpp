#include "dealDialog.h"
#include "ui_dealDialog.h"

dealDialog::dealDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dealDialog)
{
    ui->setupUi(this);
    connect(ui->accept_btn, &QPushButton::clicked, [=](){emit finished(true);});
    connect(ui->reject_btn, &QPushButton::clicked, [=](){emit finished(false);});

    disableBtn();
}

dealDialog::~dealDialog()
{
    delete ui;
}

void dealDialog::updateTransaction(transaction data)
{
    ui->type_label->setText(data.selectType());
    ui->price_label->setText(QString::number(data.selectPrice(),'f', 2) + "元");
    ui->id_label->setText("id:" + data.getId());

    ui->weight_label->setText("重量:" + QString::number(data.selectWeight(),'f', 2) + "kg");
    ui->SOH_label->setText("SOH:" + QString::number(data.selectSOH()*100) + "%");
    ui->energyDensity_label->setText("能量密度:" + QString::number(data.selectEnergyDensity(),'f', 2) + "Wh/kg");
    ui->leagcyElectricity_label->setText("剩余电量:" + QString::number(data.selectLeagcyElectricity(),'f', 2) + "度");
    ui->usage_label->setText("回收用途:" + data.selectUsagePurpose());
    ui->sellingWay_label->setText(QString("出售方式:%1")
                                  .arg(data.selectSellingWay() == "offline" ? "上门回收" : "线上邮寄"));

    QDateTime submit = data.selectSubmittedTime();
    QDateTime result = data.selectResultTime();
    if(!submit.isValid())
    {
        QMessageBox::warning(this,"警告","无法获取交易时间");
        return;
    }
    ui->submittedTime_label->setText(submit.toString("yyyy-MM-dd hh:mm"));
    if(!result.isValid())
    {
        ui->transactionStatus_label->setText("交易状态:处理中");
        ui->resultTime_label->setText("处理时间");
        enableBtn();
        return;
    }
    ui->transactionStatus_label->setText(QString("交易状态:%1").arg(data.checkStatus()? "已完成" : "被拒绝"));
    ui->resultTime_label->setText(result.toString("yyyy-MM-dd hh:mm"));
    disableBtn();
}

void dealDialog::disableBtn()
{
    ui->accept_btn->setEnabled(false);
    ui->reject_btn->setEnabled(false);
}

void dealDialog::enableBtn()
{
    ui->accept_btn->setEnabled(true);
    ui->reject_btn->setEnabled(true);
}
