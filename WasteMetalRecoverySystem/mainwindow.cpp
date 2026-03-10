#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //dialog init
    transactionHistory_dialog = new transactionHistoryDialog(this);
    transactionHistory_dialog->hide();
    transactionHistory_dialog->setWindowIcon(QIcon(":/images/res/transaction.ico"));

    setting_dialog = new settingDialog(this);
    setting_dialog->hide();
    setting_dialog->setWindowIcon(QIcon(":/images/res/gear.ico"));
    QFile file(":/QSS/setting_stylesheet.qss");
        if (file.open(QFile::ReadOnly)) {
            QString styleSheet = QLatin1String(file.readAll());
            setting_dialog->setStyleSheet(styleSheet);
            file.close();
        }
    connect(setting_dialog,&settingDialog::settingChanged,this,&MainWindow::onNewSetting);

    //timer init
    socketConnectingTimer = new QTimer(this);
    times = 0;
    connect(socketConnectingTimer, &QTimer::timeout, this, &MainWindow::socketConnectingTimer_timeout);

    transactionReceivedTimer = new QTimer(this);
    connect(transactionReceivedTimer, &QTimer::timeout, this, &MainWindow::transactionLost);

    connectToServerTimer = new QTimer(this);
    connect(connectToServerTimer, &QTimer::timeout, this, &MainWindow::connectToServerTimer_timeout);

    //socket init
    socket = new QTcpSocket(this);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MainWindow::socketError);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);
    connect(socket,&QTcpSocket::connected,this,&MainWindow::socketConnected);
    connect(socket, &QTcpSocket::readyRead,this,&MainWindow::msgFromServer);
    socketConnectToServer();

    //connect signals
    connect(ui->SOH_bar, &QSlider::valueChanged,this, &MainWindow::onSlideValueChanged);
    connect(ui->weight_spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::offFocus);
    connect(ui->SOH_bar, &QSlider::sliderReleased, this, &MainWindow::offFocus);
    connect(ui->energyDensity_spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::offFocus);
    connect(ui->type_line, &QComboBox::currentTextChanged, this, &MainWindow::comboBoxchanged);

    connect(ui->transactionHistory_frame, &interactableFrame::clicked,[=](){this->frameClicked("transactionHistory");});
    connect(ui->setting_frame, &interactableFrame::clicked,[=](){this->frameClicked("setting");});

    connect(ui->sellButton_offline, &QPushButton::clicked, [=](){sellButtonClicked("offline");});
    connect(ui->sellButton_online, &QPushButton::clicked, [=](){sellButtonClicked("online");});
    connect(ui->connectBtn, &QPushButton::clicked, this, &MainWindow::connectBtnClicked);

    //label cannot block mouse release
    ui->transactionHistory_label->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->setting_label->setAttribute(Qt::WA_TransparentForMouseEvents);

    //setting loaded
    setting = setting_dialog->readSettingFromLocal();
    transactionHistory_dialog->filePath = setting.transactionPath;
    transactionHistory_dialog->init();

    //initializaiton
    resizeWindow();
    init();
    updateTypeComboBox();
    updateMetalPrice(quo.getMetalPrice());

    polishInterface();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::polishInterface()
{
    //set style
    //top
    ui->frame_7->setProperty("type", "priceCard");
    ui->frame_8->setProperty("type", "priceCard");
    ui->frame_9->setProperty("type", "priceCard");
    ui->frame_10->setProperty("type", "priceCard");
    ui->frame_11->setProperty("type", "priceCard");
    setupCardShadow(ui->frame_7);
    setupCardShadow(ui->frame_8);
    setupCardShadow(ui->frame_9);
    setupCardShadow(ui->frame_10);
    setupCardShadow(ui->frame_11);
    setupCardShadow(ui->frame_2);

    //middle
    ui->li_price->setProperty("type", "metalPrice");
    ui->co_price->setProperty("type", "metalPrice");
    ui->mn_price->setProperty("type", "metalPrice");
    ui->ni_price->setProperty("type", "metalPrice");
    ui->cu_price->setProperty("type", "metalPrice");

    ui->label_4->setProperty("type", "subtitle");
    ui->label_9->setProperty("type", "subtitle");
    ui->label_10->setProperty("type", "subtitle");

    ui->SOH_capcity->setProperty("status", "low");

    //bottom
    ui->frame_5->setProperty("type", "infoFrame");
    ui->frame_6->setProperty("type", "infoFrame");
    ui->label_19->setProperty("type", "infoFrame_subtitle");
    ui->label_21->setProperty("type", "infoFrame_subtitle");
    ui->usagePurpose->setProperty("type", "infoFrame_value");
    ui->leagcyElectricity->setProperty("type", "infoFrame_value");

    //right
    ui->transactionHistory_frame->setProperty("type", "sideCard");
    ui->setting_frame->setProperty("type", "sideCard");
    ui->transactionHistory_label->setProperty("type", "sideCard_text");
    ui->setting_label->setProperty("type", "sideCard_text");
}

void MainWindow::setupCardShadow(QWidget *card) {
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();

    shadow->setBlurRadius(15);      // 阴影模糊半径，越大越柔和
    shadow->setColor(QColor(0, 0, 0, 40)); // 黑色阴影，透明度设为 40 (约 15%)
    shadow->setOffset(0, 4);        // 阴影向下方偏移 4 像素，产生浮动感

    card->setGraphicsEffect(shadow);
}

void MainWindow::init()
{
    ui->weight_spinBox->setValue(0.0);
    ui->energyDensity_spinBox->setValue(0.0);
    ui->final_price->setText(0);
    ui->usagePurpose->setText("未评估");
    ui->leagcyElectricity->setText("未评估");
    ui->SOH_bar->setValue(0);
    ui->SOH_bar->setRange(0,100);

    //check transaction directory
    QDir dir;
    if(!dir.exists(setting.transactionPath))
        makeDirPath(setting.transactionPath);
}

void MainWindow::resizeWindow()
{
    this->resize(setting.width,setting.height);
}

void MainWindow::updateTypeComboBox()
{
    ui->type_line->clear();

    QList<QString> batteries = quo.readAllBatteryType();
    for(auto battery : batteries)
    {
        if(quo.fetchRecoveryCostByKey(battery).isUpdated)
            ui->type_line->addItem(battery);
    }
}

bool MainWindow::clearDir(QString dirPath)
{
    QDir dir(dirPath);
        if (!dir.exists()) {
            return false;
        }

        QFileInfoList entries = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);

        bool success = true;
        for (const QFileInfo &entryInfo : entries) {
            if (!QFile::remove(entryInfo.absoluteFilePath())) {
                success = false;
            }
        }
        return success;
}

void MainWindow::updateMetalPrice(metalPrice data)
{
    ui->li_price->setText(QString::number(int(data.liPrice)) + "元/吨");
    ui->co_price->setText(QString::number(int(data.coPrice)) + "元/吨");
    ui->ni_price->setText(QString::number(int(data.niPrice)) + "元/吨");
    ui->mn_price->setText(QString::number(int(data.mnPrice)) + "元/吨");
    ui->cu_price->setText(QString::number(int(data.cuPrice)) + "元/吨");
}

//SOH bar value changed slot
void MainWindow::onSlideValueChanged(int value)
{
    QString status;

        if (value >= 80) {
            status = "high";
        } else if (value >= 20) {
            status = "mid";
        } else {
            status = "low";
        }

        ui->SOH_capcity->setText(QString("剩余电池容量：%1%").arg(value));

        ui->SOH_capcity->setProperty("status", status);

        ui->SOH_capcity->style()->unpolish(ui->SOH_capcity);
        ui->SOH_capcity->style()->polish(ui->SOH_capcity);
}
//combo box value changed
void MainWindow::comboBoxchanged()
{
    init();
}
//sell button clicked
void MainWindow::sellButtonClicked(QString sellingWay)
{
    if(!isConnectted)
    {
        QMessageBox::warning(this,"警告","未连接服务器，无法发送交易!");
        return;
    }

     double weight = ui->weight_spinBox->value();
     QString text_SOH = ui->SOH_capcity->text();

    if(weight <=0.0 || text_SOH.isEmpty())
    {
        QMessageBox::warning(this, "提示", "请先评估价格");
        return;
    }

    double energyDensity = ui->energyDensity_spinBox->value();
    QString type = ui->type_line->currentText();
    double SOH = fetchNumberFromString(text_SOH) / 100;

    QString text_price = ui->final_price->text();
    text_price.chop(1);
    double price = fetchNumberFromString(text_price);

    QString usagePurpose = ui->usagePurpose->text();
    double leagcyElectricity = energyDensity * SOH * weight;

    //creat transaction class
    transaction transactionDetails(type);
    transactionDetails.setSOH(SOH);
    transactionDetails.setPrice(price);
    transactionDetails.setWeight(weight);
    transactionDetails.setEnergyDensity(energyDensity);
    transactionDetails.setUsagePurpose(usagePurpose);
    transactionDetails.setSellingWay(sellingWay);
    transactionDetails.setLeagcyElectricity(leagcyElectricity);
    transactionDetails.setUuid(getUUID());

    QString filePath = QString(setting.transactionPath +"/%1.dat").arg(transactionDetails.getId());
    transactionDetails.setFilePath(filePath);

    transactionReceivedTimer->start(1000* 10);
    newTransaction(transactionDetails);

    transactionHistory_dialog->init();

    init();
}

void MainWindow::newTransaction(transaction data)
{
    QFile file(data.selectFilePath());
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "错误", "无法读取本地文件！");
        return;
    }

    //start out
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out << data;

    file.close();
    sendMsgToServer(NEW_TRANSACTION, data);

}

void MainWindow::onNewSetting(clientSetting setting)
{
    if(setting.transactionPath != this->setting.transactionPath)
    {
        dirPathChanged(this->setting.transactionPath,setting.transactionPath);
    }

    if(setting.width != this->setting.width || setting.height != this->setting.height)
    {
        this->setting = setting;
        resizeWindow();
    }
    this->setting = setting;
}

void MainWindow::sendMsgToServer(int type, transaction data)
{
    QByteArray block;
    QDataStream out_server(&block, QIODevice::WriteOnly);
    out_server.setVersion(QDataStream::Qt_5_14);
    out_server << type << data;
    socket->write(block);
    socket->flush(); //
}

void MainWindow::sendMsgToServer(int type)
{
    QByteArray block;
    QDataStream out_server(&block, QIODevice::WriteOnly);
    out_server.setVersion(QDataStream::Qt_5_14);
    if(type == HEART_BEAT)
        out_server << type;

    socket->write(block);
    socket->flush(); //
}

void MainWindow::makeDirPath(QString filePath)
{
    QDir dir;
    if (dir.mkpath(filePath)) {
        qDebug() << "Dir created";
    }

}

void MainWindow::offFocus()
{
    QString text_SOH = ui->SOH_capcity->text();

    double weight = ui->weight_spinBox->value();

    double energyDensity = ui->energyDensity_spinBox->value();

    QString type = ui->type_line->currentText();



    if(weight <=0.0 || text_SOH.isEmpty())
    {
        ui->final_price->setText(0);
        return;
    }


    double SOH = fetchNumberFromString(text_SOH) / 100;
    double leagcyElectricity = energyDensity * SOH * weight;
    if(SOH >= 0.8 && energyDensity >0)
        ui->usagePurpose->setText("梯次回收利用");
    else
        ui->usagePurpose->setText("金属回收");

    if(energyDensity > 0 && SOH > 0 && weight>0)
    {
        ui->leagcyElectricity->setText(QString("剩余%1度电").arg(leagcyElectricity, 0, 'f', 2));
    }

    double finalPrice = quo.quotationCaculator(type, energyDensity, weight, SOH);

    QString str = QString::number(finalPrice,'f', 2);

    ui->final_price->setText(str + "元");
}

double MainWindow::fetchNumberFromString(QString str)
{
    QRegularExpression re("\\d+\\.?\\d*");
    QRegularExpressionMatch match = re.match(str);
    double price = 0.00;

    if (match.hasMatch()) {
        QString result = match.captured(0);
        price = result.toDouble();
    }
    return price;
}

void MainWindow::frameClicked(QString frameType)
{
    if(frameType == "transactionHistory")
    {
        transactionHistory_dialog->show();
    }
    else if(frameType == "setting")
    {
        setting_dialog->show();
        setting_dialog->setDefult(setting);
    }
}

void MainWindow::transactionLost()
{
    QMessageBox::critical(this,"错误","订单无法提交！请检查服务器连接状态后重新提交！");
    transactionReceivedTimer->stop();
}

void MainWindow::updateTransaction(transaction data)
{
    QFile file(data.selectFilePath());
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "错误", "无法更新订单状态！");
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_14);
    out << data;
    file.close();
}

QString MainWindow::getUUID() {
    //generate uuid
    QSettings settings("config.ini", QSettings::IniFormat);
    QString uuid = settings.value("Device/UUID").toString();

    if (uuid.isEmpty()) {
        uuid = QUuid::createUuid().toString();
        settings.setValue("Device/UUID", uuid);
    }
    return uuid;
}

bool MainWindow::dirPathChanged(QString oldPath, QString newPath)
{
    QDir oldDir(oldPath);
        if (!oldDir.exists()) return false;

        if(!QDir().exists(newPath))
            QDir().mkpath(newPath);

        QStringList files = oldDir.entryList(QDir::Files);

        for (const QString &fileName : files) {
            QString oldFile = oldPath + "/" + fileName;
            QString newFile = newPath + "/" + fileName;

            if (QFile::exists(newFile)) {
                QFile::remove(newFile);
            }

            if (!QFile::rename(oldFile, newFile)) {
                qDebug() << "文件移动失败: " << fileName;
            }
        }
        return true;
}

void MainWindow::startHandshake()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_14);

    out << HANDSHAKE;
    out << getUUID();

    socket->write(block);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "确认", "确定要退出程序吗？",
                                                               QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
        return;
    }
    if (socket && socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
    event->accept();
}

//socket-server function
void MainWindow::socketError(QAbstractSocket::SocketError socketError)
{
    connectToServerTimer->stop();
    socketConnectingTimer->stop();
    times = 0;
    qDebug()<<socketError;
    ui->socketStatus_label->setText("🔴未连接");
    ui->connectBtn->setEnabled(true);
}

void MainWindow::socketDisconnected()
{
    ui->socketStatus_label->setText("🔴未连接");
    ui->connectBtn->setEnabled(true);

}

void MainWindow::socketConnected()
{
    startHandshake();
    ui->socketStatus_label->setText("🟢已连接");
    isConnectted = true;
    socketConnectingTimer->stop();
    connectToServerTimer->stop();
}

void MainWindow::socketConnectToServer()
{
    isConnectted = false;
    socketConnectingTimer->start(1000);
    socket->connectToHost(setting.ip, setting.port);
    ui->connectBtn->setEnabled(false);
    connectToServerTimer->start(1000 * setting.waittingTime);
}

void MainWindow::connectToServerTimer_timeout()
{
    connectToServerTimer->stop();
    socketConnectingTimer->stop();
    times = 0;
    if (socket->state() != QAbstractSocket::ConnectedState) {
            socket->abort(); // 强制终止连接尝试
            ui->socketStatus_label->setText("🔴未连接");
            ui->connectBtn->setEnabled(true);
        }
}

void MainWindow::socketConnectingTimer_timeout()
{
    times++;
    if(times > 4)
        times = 0;
    else if (times == 1)
        ui->socketStatus_label->setText("🟡连接中");
    else if (times == 2)
        ui->socketStatus_label->setText("🟡连接中.");
    else if (times == 3)
        ui->socketStatus_label->setText("🟡连接中..");
    else if (times == 4)
        ui->socketStatus_label->setText("🟡连接中...");
}

void MainWindow::connectBtnClicked()
{
    if(ui->connectBtn->text() == "重新连接")
        socketConnectToServer();
}

void MainWindow::msgFromServer()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_14);
    while(true)
    {
        in.startTransaction();

        int order;
        in>>order;
        QDateTime newestTime;

        if(order == HANDSHAKE)    //set up initialization
        {
            metalPrice metal_price;
            QList<QString> batteries_list;
            QList<batteryMaterialConcentration> materialConcentration_list;
            QList<recoveryCost> recoveryCost_list;

            in >> metal_price >> batteries_list >> materialConcentration_list >> recoveryCost_list;

            if(!in.commitTransaction())
                return;

            //clear dir
            clearDir("bin/quotation_model/recoveryCost");
            clearDir("bin/quotation_model/battery");

            quo.saveMetalPriceToLocal(metal_price);
            for(int i = 0; i < batteries_list.length() && i < materialConcentration_list.length() && i < recoveryCost_list.length(); i++)
            {
                QString battery = batteries_list.at(i);
                batteryMaterialConcentration* materialConcentration = new batteryMaterialConcentration(materialConcentration_list.at(i));
                recoveryCost cost = recoveryCost_list.at(i);

                quo.saveBatteryToLocal(battery,materialConcentration);
                quo.saveRecoveryCostToLocal(battery,cost);
            }
            quo.readMetalPriceFromLocal();
            quo.readAllBatteryFromLocal();
            quo.readAllRecoveryCostFromLocal();

            //init for refreshing comboBox items
            updateTypeComboBox();
            updateMetalPrice(metal_price);

        }
        else if(order == NEW_TRANSACTION)   //transaction received
        {
            QMessageBox::information(this, "成功", "电池交易请求提交成功！");
            transactionReceivedTimer->stop();
        }
        else if(order == TRANSACTION_STATUS)   //transaction status updated
        {
            transaction data;
            in>>data;
            if(in.commitTransaction())
            {
                updateTransaction(data);
                transactionHistory_dialog->init();
            }

        }
        else if(order == METAL_PRICE) // update metal price
        {
            metalPrice data;
            in>>data;
            if(in.commitTransaction())
            {
                if(data.isUpdated)
                {
                    quo.saveMetalPriceToLocal(data);
                    quo.readMetalPriceFromLocal();
                    updateMetalPrice(quo.getMetalPrice());
                }
            }
        }
        else if(order == HEART_BEAT) // test heart beat
        {
            sendMsgToServer(HEART_BEAT);
        }
        else if(order == QUOTATION_DATA)//New quotation data
        {
            QString battery;
            batteryMaterialConcentration materialConcentration;
            recoveryCost cost;
            in>>battery >> materialConcentration >> cost;
            if(!in.commitTransaction())return;
            batteryMaterialConcentration* data = new batteryMaterialConcentration(materialConcentration);
            if(cost.isUpdated)
            {
                quo.changeRecoveryCostValue(battery,cost);
                quo.changeBatteryValue(battery,data);
                updateTypeComboBox();
                QMessageBox::information(this,"提示","收到服务器的新消息：电池材料["+battery+"]报价参数已修改！");
            }
            else
            {
                quo.addRecoveryCost(battery, cost);
                quo.addBatteryType(battery,data);
            }
            quo.saveRecoveryCostToLocal(battery, cost);
            quo.saveBatteryToLocal(battery, data);

        }
        else if(order == BATTERY_REMOVED)// delete a battery
        {
            QString battery;
            in>>battery;
            if(!in.commitTransaction())return;
            quo.removeBatteryByName(battery);
            quo.removeBatteryFromLocal(battery);

            if(quo.fetchRecoveryCostByKey(battery).isUpdated)
            {
                updateTypeComboBox();
                QMessageBox::information(this,"提示","收到服务器的新消息：["+battery+"]被移除！");
            }

        }
        else if(order == BATTERY_CHANGED)
        {
            QString oldKey;
            QString newKey;
            in>>oldKey>>newKey;

            if(!in.commitTransaction()||oldKey.isEmpty() || newKey.isEmpty())return;

            quo.changeBatteryNameKey(newKey,oldKey);
            quo.changeRecoveryCostKey(newKey,oldKey);

            quo.renameLocalBattery(oldKey,newKey);
            quo.renameLocalRecoveryCost(oldKey,newKey);

            if(quo.fetchRecoveryCostByKey(oldKey).isUpdated)
            {
                updateTypeComboBox();
                QMessageBox::information(this,"提示","收到服务器的新消息：["+oldKey+"]改名为["+newKey+"]！");
            }

        }
        else
        {
            in.rollbackTransaction();
            break;
        }
    }
}
