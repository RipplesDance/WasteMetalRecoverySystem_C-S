#include "onlineClientsDialog.h"
#include "ui_onlineClientsDialog.h"

onlineClientsDialog::onlineClientsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::onlineClientsDialog)
{
    ui->setupUi(this);
    sequence=0;
    connect(ui->heartBeat_btn,&QPushButton::clicked,this,&onlineClientsDialog::heartBeat);
    init();
}

onlineClientsDialog::~onlineClientsDialog()
{
    delete ui;
}

void onlineClientsDialog::init()
{
    ui->clientsInfo_treeWidget->setColumnWidth(0,200);
    ui->clientsInfo_treeWidget->setColumnWidth(1,150);
    ui->clientsInfo_treeWidget->setColumnWidth(2,300);
    ui->clientsInfo_treeWidget->setColumnWidth(3,550);
}

void onlineClientsDialog::addClient(clientInfo* data)
{
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->clientsInfo_treeWidget);

    item->setText(0,QString::number(++sequence));
    item->setText(1,data->getIp());
    item->setText(2,data->selectLoginTime().toString("yyyy-MM-dd hh:mm:ss"));
    item->setText(3,data->getUuid());
}

void onlineClientsDialog::updateClientNumber(int n)
{
    ui->clientsNumber_label->setText(QString("当前在线人数:%1").arg(n));
}

void onlineClientsDialog::clearTreeWidget()
{
    sequence = 0;
    ui->clientsInfo_treeWidget->clear();
}
