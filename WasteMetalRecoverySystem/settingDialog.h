#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include<QDebug>
#include<QFile>
#include<QDir>
#include<QDataStream>
#include"clientSetting.h"

namespace Ui {
class settingDialog;
}

class settingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingDialog(QWidget *parent = nullptr);
    ~settingDialog();
    void setDefult(clientSetting setting);
    bool saveSettingToLocal(clientSetting setting);
    clientSetting readSettingFromLocal();

public slots:
    void offFocus();

signals:
    void settingChanged(clientSetting setting);

private:
    Ui::settingDialog *ui;
    QString settingPath = "bin/setting";
};

#endif // SETTINGDIALOG_H
