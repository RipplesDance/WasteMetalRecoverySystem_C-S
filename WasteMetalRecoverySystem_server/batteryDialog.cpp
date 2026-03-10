#include "batteryDialog.h"
#include "ui_batteryDialog.h"

batteryDialog::batteryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::batteryDialog)
{
    ui->setupUi(this);
    init();
    connect(ui->batteryInfo_treeWidget, &QTreeWidget::customContextMenuRequested, this, &batteryDialog::showContextMenu);
    connect(ui->batteryInfo_treeWidget, &QTreeWidget::itemChanged, this, &batteryDialog::handleItemChanged);
}

void batteryDialog::init()
{
    for(int i = 0; i < ui->batteryInfo_treeWidget->columnCount(); ++i) {
        ui->batteryInfo_treeWidget->resizeColumnToContents(i);
    }
    ui->batteryInfo_treeWidget->setColumnWidth(0,200);
}

void batteryDialog::handleItemChanged(QTreeWidgetItem *item, int column)
{
    if(!itemMap.contains(item))return;
    QString oldKey = itemMap.value(item);
    QString newKey = item->text(0);
    if(oldKey == newKey)
    {
        //value changed
        if(item->text(column).isEmpty())
        {
            ui->batteryInfo_treeWidget->blockSignals(true);
            item->setText(column, "0%");
            ui->batteryInfo_treeWidget->blockSignals(false);
        }

        //set range
        QString rawText = item->text(column);
        double num = fetchNumberFromString(rawText);
        if (num < 0) num = 0;
        if (num > 100) num = 100;
        ui->batteryInfo_treeWidget->blockSignals(true);
        item->setText(column, QString::number(num) + "%");
        ui->batteryInfo_treeWidget->blockSignals(false);

        batteryMaterialConcentration* value = fetchDataFromItem(item);
        qDebug()<<"batteryDialog:batteryChanged " << value->li_recycleRatio;
        emit batteryChanged(oldKey, value);
        return;
    }
    //else battery name changed
    if (newKey.isEmpty()|| isBatteryNameExists(newKey) || newKey == "新电池材料") {
        QMessageBox::warning(this,"警告","新名字已经存在或违法！");
        ui->batteryInfo_treeWidget->blockSignals(true);
        item->setText(0, oldKey);
        ui->batteryInfo_treeWidget->blockSignals(false);
        return;
    }
    itemMap[item] = newKey;
    qDebug()<<"batteryDialog:batteryNameChanged";
    emit batteryNameChanged(newKey, oldKey);

}

double batteryDialog::fetchNumberFromString(QString str)
{
    QRegularExpression re("\\d+\\.?\\d*");
    QRegularExpressionMatch match = re.match(str);
    double num = 0.00;

    if (match.hasMatch()) {
        QString result = match.captured(0);
        num = result.toDouble();
    }
    return num;
}

batteryMaterialConcentration* batteryDialog::fetchDataFromItem(QTreeWidgetItem *item)
{
    batteryMaterialConcentration* data = new batteryMaterialConcentration;
    data->positiveMaterialsRatio = fetchNumberFromString(item->text(1))/100;
    data->positiveMaterial_recycleRatio = fetchNumberFromString(item->text(2))/100;
    data->compoundRatio = fetchNumberFromString(item->text(3))/100;
    data->compound_recycleRatio = fetchNumberFromString(item->text(4))/100;
    data->li = fetchNumberFromString(item->text(5))/100;
    data->li_recycleRatio = fetchNumberFromString(item->text(6))/100;
    data->co = fetchNumberFromString(item->text(7))/100;
    data->co_recycleRatio = fetchNumberFromString(item->text(8))/100;
    data->ni = fetchNumberFromString(item->text(9))/100;
    data->ni_recycleRatio = fetchNumberFromString(item->text(10))/100;
    data->mn = fetchNumberFromString(item->text(11))/100;
    data->mn_recycleRatio = fetchNumberFromString(item->text(12))/100;
    data->cu = fetchNumberFromString(item->text(13))/100;
    data->cu_recycleRatio = fetchNumberFromString(item->text(14))/100;
    return data;
}

bool batteryDialog::isBatteryNameExists(QString str)
{
    QMap<QTreeWidgetItem*, QString>::iterator it = itemMap.begin();
    while (it != itemMap.end()) {
        if(it.value() == str)
            return true;
        it++;
    }
    return false;
}

void batteryDialog::showContextMenu(QPoint pos)
{
    QMenu menu(this);

    QAction *addAction = menu.addAction("添加新电池材料");

    QTreeWidgetItem *curItem = ui->batteryInfo_treeWidget->itemAt(pos);
    QAction *delAction = nullptr;

    if (curItem) {
        delAction = menu.addAction("删除当前电池材料");
    }

    QAction *selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == addAction) {
        addNewItem();
    } else if (curItem && selectedAction == delAction) {
        deleteSelectedItem(curItem);
    }
}

void batteryDialog::addNewItem() {

    if(isBatteryNameExists("新电池材料"))
    {
        QMessageBox::warning(this,"警告","当前已新建一个项，无法创建更多！");
        return;
    }

    ui->batteryInfo_treeWidget->blockSignals(true);

    QTreeWidgetItem *newItem = new QTreeWidgetItem(ui->batteryInfo_treeWidget);
    newItem->setText(0, "新电池材料");
    newItem->setText(1, QString::number(0)+"%");
    newItem->setText(2, QString::number(0)+"%");
    newItem->setText(3, QString::number(0)+"%");
    newItem->setText(4, QString::number(0)+"%");
    newItem->setText(5, QString::number(0)+"%");
    newItem->setText(6, QString::number(0)+"%");
    newItem->setText(7, QString::number(0)+"%");
    newItem->setText(8, QString::number(0)+"%");
    newItem->setText(9, QString::number(0)+"%");
    newItem->setText(10, QString::number(0)+"%");
    newItem->setText(11, QString::number(0)+"%");
    newItem->setText(12, QString::number(0)+"%");
    newItem->setText(13, QString::number(0)+"%");
    newItem->setText(14, QString::number(0)+"%");

    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);

    itemMap.insert(newItem,"新电池材料");

    ui->batteryInfo_treeWidget->addTopLevelItem(newItem);
    ui->batteryInfo_treeWidget->editItem(newItem, 0); // edit immediately
    ui->batteryInfo_treeWidget->blockSignals(false);
    batteryMaterialConcentration* data = new batteryMaterialConcentration;
    emit newBattery("新电池材料", data);
}

void batteryDialog::deleteSelectedItem(QTreeWidgetItem *item) {
    if (!item) return;

    QString key = item->text(0);

    auto result = QMessageBox::question(this, "确认删除", QString("确定要删除电池材料 [%1] 吗？").arg(key));
    if (result == QMessageBox::Yes) {

        itemMap.remove(item);
        delete item;
        emit removeBattery(key);
    }
}

void batteryDialog::clearTreeWidget()
{
    itemMap.clear();
    ui->batteryInfo_treeWidget->clear();
}

void batteryDialog::addItemToTreeWidget(QString key, batteryMaterialConcentration* data)
{
    ui->batteryInfo_treeWidget->blockSignals(true);

    QTreeWidgetItem* item = new QTreeWidgetItem(ui->batteryInfo_treeWidget);
    item->setText(0, key);
    item->setText(1, QString::number(data->positiveMaterialsRatio*100)+"%");
    item->setText(2, QString::number(data->positiveMaterial_recycleRatio*100)+"%");
    item->setText(3, QString::number(data->compoundRatio*100)+"%");
    item->setText(4, QString::number(data->compound_recycleRatio*100)+"%");
    item->setText(5, QString::number(data->li*100)+"%");
    item->setText(6, QString::number(data->li_recycleRatio*100)+"%");
    item->setText(7, QString::number(data->co*100)+"%");
    item->setText(8, QString::number(data->co_recycleRatio*100)+"%");
    item->setText(9, QString::number(data->ni*100)+"%");
    item->setText(10, QString::number(data->ni_recycleRatio*100)+"%");
    item->setText(11, QString::number(data->mn*100)+"%");
    item->setText(12, QString::number(data->mn_recycleRatio*100)+"%");
    item->setText(13, QString::number(data->cu*100)+"%");
    item->setText(14, QString::number(data->cu_recycleRatio*100)+"%");

    item->setFlags(item->flags() | Qt::ItemIsEditable);

    itemMap.insert(item,key);
    ui->batteryInfo_treeWidget->blockSignals(false);

}

batteryDialog::~batteryDialog()
{
    delete ui;
}
