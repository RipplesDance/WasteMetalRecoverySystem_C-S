#include "transactionHistoryDialog.h"
#include "ui_transactionHistoryDialog.h"

transactionHistoryDialog::transactionHistoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::transactionHistoryDialog)
{
    ui->setupUi(this);

    ui->sort_box->addItem("按订单创建时间正序");
    ui->sort_box->addItem("按订单创建时间倒序");
    ui->sort_box->addItem("按报价大小正序");
    ui->sort_box->addItem("按报价大小倒序");

    connect(ui->sort_box, &QComboBox::currentTextChanged, this, &transactionHistoryDialog::sortBoxChanged);
    connect(ui->transactionList, &QListWidget::itemClicked, this, &transactionHistoryDialog::selectedItem);

    ui->transactionList->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->transactionList, &QListWidget::customContextMenuRequested,
            this, &transactionHistoryDialog::showTransactionContextMenu);

    //set style
    ui->transactionList->setSpacing(10);

//    ui->transactionList->setStyleSheet(
//        "QListWidget {"
//        "    outline: none;"
//        "}"

//        "QListWidget::item {"
//        "    color: #333333;"
//        "    padding: 10px;"
//        "}"
//    );

//    ui->frame->setStyleSheet(".QFrame { border-radius: 10px; border: 2px inset #828282; }");
}

transactionHistoryDialog::~transactionHistoryDialog()
{
    delete ui;
}

void transactionHistoryDialog::init()
{
    if(filePath.isEmpty()) return;

    QDir dir(filePath);

    //set filter
    QStringList filter;
    filter<< "*.dat";
    dir.setNameFilters(filter);

    //start reading file
    QFileInfoList fileList = dir.entryInfoList(QDir::Files | QDir::NoDot | QDir::NoDotDot);

    fileVector.clear();
    for(const QFileInfo &fileInfo : fileList)
    {
        QFile file(fileInfo.absoluteFilePath());
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(this,"错误","文件无法读取!");
            return;
        }
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_14);
        transaction data;
        in >> data;
        fileVector.push_back(data);

        file.close();
    }

    sortBoxChanged(ui->sort_box->currentText());
}

void transactionHistoryDialog::showTransactionContextMenu(const QPoint &pos)
{
    QListWidget *list = ui->transactionList;
    QListWidgetItem *item = list->itemAt(pos);

    // 如果没点到任何 item，不弹出菜单
    if (!item) {
        return;
    }

    // 获取该 item 对应的文件路径（你已经设置了 Qt::UserRole）
    QString filePath = item->data(Qt::UserRole).toString();
    if (filePath.isEmpty()) {
        QMessageBox::warning(this, "提示", "该订单缺少文件路径信息，无法操作");
        return;
    }
    QMenu contextMenu(this);
     QAction *deleteAction = contextMenu.addAction(QIcon(":/images/res/delete.ico"), "删除该订单");

    QAction *selected = contextMenu.exec(list->viewport()->mapToGlobal(pos));

    if (selected == deleteAction) {
        QMessageBox::StandardButton reply = QMessageBox::question(
                    this,
                    "确认删除",
                    QString("确定要删除订单：\n%1\n此操作无法恢复！").arg(filePath),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No
                    );

        if (reply != QMessageBox::Yes) {
            return;
        }

        // remove from vector
        auto it = std::find_if(fileVector.begin(), fileVector.end(),
                               [&filePath](transaction &t) {
            return t.selectFilePath() == filePath;
        });
        //find it in vector
        if (it != fileVector.end()) {
            fileVector.erase(it);
        } else {
            QMessageBox::warning(this, "异常", "在数据列表中未找到该订单");
        }


        // remove from list widget
        int row = list->row(item);
        delete list->takeItem(row);
        sortBoxChanged(ui->sort_box->currentText());

        // remove from local
        QFile file(filePath);
        if (file.exists()) {
            if (!file.remove()) {
                QMessageBox::warning(this, "文件删除失败",
                                     "订单数据已从列表移除，但本地文件删除失败:\n" + filePath);
            }
        }
    }
}

void transactionHistoryDialog::updataTransaction(transaction data)
{
    ui->type_label->setText(data.selectType());
    ui->price_label->setText(QString::number(data.selectPrice(),'f', 2) + "元");
    ui->id_label->setText("【<img src=':/images/res/id.ico' width='18' height='18'/> id:" + data.getId()+" 】");

    ui->weight_label->setText("【<img src=':/images/res/weight.ico' width='18' height='18'/> 重量:" + QString::number(data.selectWeight(),'f', 2) + "kg 】");
    ui->SOH_label->setText("【<img src=':/images/res/SOH.ico' width='18' height='18'/> SOH:" + QString::number(data.selectSOH()*100) + "% 】");
    ui->energyDensity_label->setText("【<img src=':/images/res/energyDensity.ico' width='18' height='18'/> 能量密度:" + QString::number(data.selectEnergyDensity(),'f', 2) + "Wh/kg 】");
    ui->leagcyElectricity_label->setText("【<img src=':/images/res/leagcyElectricity.ico' width='18' height='18'/> 剩余电量:" + QString::number(data.selectLeagcyElectricity(),'f', 2) + "度 】");
    ui->usage_label->setText("【<img src=':/images/res/recycle.ico' width='18' height='18'/> 回收用途:" + data.selectUsagePurpose() + " 】");
    ui->sellingWay_label->setText(QString("【<img src=':/images/res/package.ico' width='18' height='18'/> 出售方式:%1")
                                  .arg(data.selectSellingWay() == "offline" ? "上门回收 】" : "线上邮寄 】"));

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
        ui->duration_label->setText("<img src=':/images/res/duration.ico' width='14' height='14'/> 耗时:");
        ui->transactionStatus_label->setStyleSheet(
                    "QLabel {"
                    "padding: 4px 12px;"
                    "border-radius: 10px;"
                    "background-color: #2196F3;"
                    "color: #fff;"
                    "font-weight: bold;"
                    "max-width: 210px;"
                "}"
                    );
        ui->resultTime_label->setStyleSheet(""
                                            "QLabel {"
                                            "max-width: 210px;"
                                            "}");
        return;
    }
    ui->transactionStatus_label->setText(QString("交易状态:%1").arg(data.checkStatus()? "已完成" : "被拒绝"));
    if(data.checkStatus()){
        ui->transactionStatus_label->setStyleSheet(
                    "QLabel {"
                    "padding: 4px 12px;"
                    "border-radius: 10px;"
                    "background-color: #4CAF50;"
                    "color: #fff;"
                    "font-weight: bold;"
                    "max-width: 210px;"
                "}"
                    );

        ui->resultTime_label->setStyleSheet(
                    "QLabel {"
                    "border: none;"
                    "border-bottom: 1px solid #4CAF50;"
                    "max-width: 210px;"
                "}"
                    );
    }
    else{
        ui->transactionStatus_label->setStyleSheet(
                    "QLabel {"
                    "padding: 4px 12px;"
                    "border-radius: 10px;"
                    "background-color: #F44336;"
                    "color: #fff;"
                    "font-weight: bold;"
                    "max-width: 210px;"
                "}"
                    );
        ui->resultTime_label->setStyleSheet(
                    "QLabel {"
                    "border: none;"
                    "border-bottom: 1px solid #F44336;"
                    "max-width: 210px;"
                "}"
                    );
    }
    ui->resultTime_label->setText(result.toString("yyyy-MM-dd hh:mm"));

    qint64 totalSecs = submit.secsTo(result);
    int hours = totalSecs / 3600;
    int minutes = (totalSecs % 3600) / 60;
    ui->duration_label->setText("<img src=':/images/res/duration.ico' width='14' height='14'/> "+QString("耗时:%1时%2分").arg(hours).arg(minutes));


}

void transactionHistoryDialog::onNewTransaction()
{
    init();
}

void transactionHistoryDialog::selectedItem(QListWidgetItem *item)
{
    if(!item) return;
    QString filePath = item->data(Qt::UserRole).toString();

    auto it = std::find_if(fileVector.begin(), fileVector.end(), [=](transaction d){
            return d.selectFilePath() == filePath;
        });

        if (it != fileVector.end()) {
            transaction clickedData = *it;
            updataTransaction(clickedData);
        }
}

void transactionHistoryDialog::updataListWidget()
{
    ui->transactionList->clear();
    for(auto data : fileVector)
    {
        QListWidgetItem* item = new QListWidgetItem(data.selectType() + "-" +
                                                    QString::number(data.selectPrice()) + "-" + data.getId());
        item->setData(Qt::UserRole, data.selectFilePath());

        if(data.selectResultTime().isValid())
        {
            if(data.checkStatus())
                item->setBackground(QColor(200, 255, 200));
            else
                item->setBackground(QColor(255, 200, 200));
        }
        else
            item->setBackground((QColor(200, 200, 255)));

        ui->transactionList->addItem(item);
    }
}

void transactionHistoryDialog::sortBoxChanged(QString way)
{
    if(way == "按订单创建时间正序")
    {
        std::sort(fileVector.begin(), fileVector.end(),
                  [=](transaction &a, transaction &b){return a.selectSubmittedTime() > b.selectSubmittedTime();});
    }
    else if(way == "按订单创建时间倒序")
    {
        std::sort(fileVector.begin(), fileVector.end(),
                  [=](transaction &a, transaction &b){return a.selectSubmittedTime() < b.selectSubmittedTime();});
    }
    else if(way == "按报价大小正序")
    {
        std::sort(fileVector.begin(), fileVector.end(),
                  [=](transaction &a, transaction &b){return a.selectPrice() > b.selectPrice();});
    }
    else if(way == "按报价大小倒序")
    {
        std::sort(fileVector.begin(), fileVector.end(),
                  [=](transaction &a, transaction &b){return a.selectPrice() < b.selectPrice();});
    }
    updataListWidget();
}
