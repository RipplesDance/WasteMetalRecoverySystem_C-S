#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include<QMessageBox>
#include<QMap>
#include<QList>
#include<QDir>
#include<QDataStream>
#include<QDateTime>
#include<QFile>
#include<QListWidgetItem>
#include<QVector>
#include<QCloseEvent>
#include<QList>
#include"transaction.h"
#include"clientInfo.h"
#include"dealDialog.h"
#include"metalPrice.h"
#include"metalPriceDialog.h"
#include"onlineClientsDialog.h"
#include"quotation.h"
#include"batteryDialog.h"
#include"quotationDialog.h"
#include"recoveryCost.h"

enum {
    HANDSHAKE = 0,
    NEW_TRANSACTION = 1,
    TRANSACTION_STATUS = 2,
    METAL_PRICE = 3,
    QUOTATION_DATA = 4,
    HEART_BEAT = 5,
    BATTERY_REMOVED = 6,
    BATTERY_CHANGED = 7,
    MESSAGE = 8
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void init();
    void newTransactionRecived(transaction data);
    void saveTransactionToLocal(transaction data);
    void sendMsgToSocket(QTcpSocket* socket, int msg_type, transaction data);
    void sendMsgToSocket(QTcpSocket* socket, int msg_type, metalPrice data);
    void sendMsgToSocket(QTcpSocket* socket, int msg_type,QString battery, batteryMaterialConcentration materialConcentration, recoveryCost cost);
    void sendMsgToSocket(QTcpSocket* socket, int msg_type, QString oldKey, QString newKey);
    void sendMsgToSocket(QTcpSocket* socket, int msg_type, QString msg);
    void sendMsgToSocket(QTcpSocket* socket, int msg_type);
    void updateLabel();
    void readLocalTransaction();
    void updateTransactionListWidget();
    QTcpSocket* fetchSocketFromUuid(QString uuid);
    void TransactionHandled(transaction data, bool isAccept);
    QString getCurrentDateTime();
    void addMsgToMsgServer(QString str);
    void writeLog(QString msg);
    void messageFromClient(QTcpSocket* socket);
    void removeZombie(clientInfo* data);

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void clientConnected();
    void clientDisconnected();
    void onNewConnection();
    void sortBoxChanged(QString way);
    void selectedItem(QListWidgetItem *item);
    void onMetalPriceFrame();
    void onOnlineClientsFrame();
    void onManageBatteryTypeFrame();
    void onUpdateQuotationFrame();
    void updateMetalPrice(metalPrice data);
    void heartBeatDetection();
    void heartBeatResult();
    void onBatteryValueChanged(QString key, batteryMaterialConcentration* value);
//    void onRecoveryCostChanged(QString key, recoveryCost data);
    void onRemoveBattery(QString key);
    void onBatteryNameChanged(QString newKey, QString oldKey);
    void onNewBattery(QString key, batteryMaterialConcentration* value);
    void onBatterySelected(QString key);
    void onTemporaryCalculator(QString type, double energyDensity, double weight, double SOH, recoveryCost cost);
    void onCostChangeConfirmed(QString key, recoveryCost value);
    void showPendingContextMenu(const QPoint &pos);
    void showFinishedContextMenu(const QPoint &pos);

private:
    Ui::MainWindow *ui;
    //network parameters
    QTcpServer *server;
    QMap<QTcpSocket*, clientInfo*> clientMap;
    //transaction parameters
    QVector<transaction> fileVector;
    QVector<transaction> finishedFileVector;
    //extra dialog parameters
    dealDialog* deal_dialog;
    metalPriceDialog* metalPrice_dialog;
    onlineClientsDialog* onlineClients_dialog;
    batteryDialog* battery_dialog;
    quotationDialog* quotation_dialog;
    //auxiliary parameters
    int zombieNumber;
    QTimer *heartBeatTimer;
    //file parameters
    QString logFilePath;
    QFile logFile;
    //quotation model parameters
    quotation quo;

};
#endif // MAINWINDOW_H
