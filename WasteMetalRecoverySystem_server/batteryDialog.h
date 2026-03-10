#ifndef BATTERYDIALOG_H
#define BATTERYDIALOG_H

#include <QDialog>
#include<QTreeWidgetItem>
#include<QPoint>
#include<QMenu>
#include<QTreeWidgetItem>
#include<QMessageBox>
#include<QRegularExpression>
#include"batteryMaterialConcentration.h"

namespace Ui {
class batteryDialog;
}

class batteryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit batteryDialog(QWidget *parent = nullptr);
    ~batteryDialog();
    void init();
    void addItemToTreeWidget(QString key, batteryMaterialConcentration* data);
    void clearTreeWidget();
    void addNewItem();
    void deleteSelectedItem(QTreeWidgetItem* item);
    void handleItemChanged(QTreeWidgetItem *item, int column);
    batteryMaterialConcentration* fetchDataFromItem(QTreeWidgetItem *item);
    double fetchNumberFromString(QString str);
    bool isBatteryNameExists(QString str);

public slots:
    void showContextMenu(QPoint pos);

signals:
    void removeBattery(QString key);
    void batteryChanged(QString key, batteryMaterialConcentration* value);
    void batteryNameChanged(QString newKey, QString oldKey);
    void newBattery(QString key, batteryMaterialConcentration* value);

private:
    Ui::batteryDialog *ui;
    QMap<QTreeWidgetItem*, QString> itemMap;

};

#endif // BATTERYDIALOG_H
