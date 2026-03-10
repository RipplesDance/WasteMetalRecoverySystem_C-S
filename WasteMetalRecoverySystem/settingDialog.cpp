#include "settingDialog.h"
#include "ui_settingDialog.h"

settingDialog::settingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingDialog)
{
    ui->setupUi(this);
    ui->ip_lineEdit->setInputMask("000.000.000.000;_");
    ui->ip_lineEdit->setText("127.0.0.1");

    QIntValidator *portValidator = new QIntValidator(0, 65535, this);
    ui->port_lineEdit->setValidator(portValidator);
    ui->port_lineEdit->setText("8888");

    //connect slots
    connect(ui->ip_lineEdit,&QLineEdit::editingFinished,this,&settingDialog::offFocus);
    connect(ui->port_lineEdit,&QLineEdit::editingFinished,this,&settingDialog::offFocus);
    connect(ui->transactionDirPath_lineEdit,&QLineEdit::editingFinished,this,&settingDialog::offFocus);
    connect(ui->width_spinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&settingDialog::offFocus);
    connect(ui->height_spinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&settingDialog::offFocus);
    connect(ui->waittingTime_spinBox,QOverload<int>::of(&QSpinBox::valueChanged),this,&settingDialog::offFocus);

    //check if setting dir exists
    QDir dir;
    if(!dir.exists(settingPath))
        if (dir.mkpath(settingPath)) {
            qDebug() << "settingPath dir created";
        }

    //check if setting file exists
    if(!QFile::exists(settingPath + "/setting.dat"))
        saveSettingToLocal(clientSetting());
}

settingDialog::~settingDialog()
{
    delete ui;
}

bool settingDialog::saveSettingToLocal(clientSetting setting)
{
    QFile file(settingPath + "/setting.dat");
    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug()<<"无法打开文件setting.dat";
        return false;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out<< setting;
    return true;
}

clientSetting settingDialog::readSettingFromLocal()
{
    QFile file(settingPath + "/setting.dat");
    clientSetting setting;
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug("无法读取本地金属价格信息!");
        return setting;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_14);
    in >> setting;

    return setting;
}

void settingDialog::offFocus()
{
    clientSetting newSetting;
    newSetting.ip = ui->ip_lineEdit->text();
    newSetting.port = ui->port_lineEdit->text().toUShort();
    newSetting.width = ui->width_spinBox->value();
    newSetting.height = ui->height_spinBox->value();
    newSetting.waittingTime = ui->waittingTime_spinBox->value();
    newSetting.transactionPath = ui->transactionDirPath_lineEdit->text();

    saveSettingToLocal(newSetting);

    emit settingChanged(newSetting);
}

void settingDialog::setDefult(clientSetting setting){
    ui->ip_lineEdit->setText(setting.ip);
    ui->port_lineEdit->setText(QString::number(setting.port));
    ui->transactionDirPath_lineEdit->setText(setting.transactionPath);
    ui->width_spinBox->setValue(setting.width);
    ui->height_spinBox->setValue(setting.height);
    ui->waittingTime_spinBox->setValue(setting.waittingTime);
}

