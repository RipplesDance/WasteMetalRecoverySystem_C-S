#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QMainWindow>
#include<QDebug>
#include<QMap>
#include<QRegularExpression>
#include<QMessageBox>
#include<QSettings>
#include<QUuid>
#include<QTimer>
#include<QFile>
#include<QDir>
#include<QDataStream>
#include <QTcpServer>
#include <QTcpSocket>
#include<QCloseEvent>
#include<QList>
#include"batteryMaterialConcentration.h"
#include"quotation.h"
#include"transaction.h"
#include"transactionHistoryDialog.h"
#include"metalPrice.h"
#include"recoveryCost.h"
#include"settingDialog.h"
#include"clientSetting.h"
#include <QGraphicsDropShadowEffect>

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
    double fetchNumberFromString(QString str);
    void makeDirPath(QString filePath);
    void sellButtonClicked(QString sellingWay);
    void frameClicked(QString frameType);
    void updateTransaction(transaction data);
    void newTransaction(transaction data);
    void sendMsgToServer(int type, transaction data);
    void sendMsgToServer(int type);
    void updateMetalPrice(metalPrice data);
    void updateTypeComboBox();
    bool clearDir(QString dirPath);
    void socketConnectToServer();
    QString getUUID();
    void resizeWindow();
    void startHandshake();
    bool dirPathChanged(QString oldPath, QString newPath);

    //polish
    void polishInterface();
    void setupCardShadow(QWidget *card);

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void onSlideValueChanged(int value);
    void offFocus();
    void comboBoxchanged();

    void socketError(QAbstractSocket::SocketError socketError);
    void socketDisconnected();
    void socketConnected();

    void socketConnectingTimer_timeout();
    void connectToServerTimer_timeout();
    void connectBtnClicked();
    void msgFromServer();
    void transactionLost();

    void onNewSetting(clientSetting setting);
signals:
    void newTransaction();


private:
    Ui::MainWindow *ui;
    QMap<QString, double> metalPriceMap;
    quotation quo;

    //extra dialog
    transactionHistoryDialog *transactionHistory_dialog;
    settingDialog* setting_dialog;

    //client parameters
    QTcpSocket *socket;
    bool isConnectted;
    QTimer *socketConnectingTimer;
    QTimer *transactionReceivedTimer;
    QTimer *connectToServerTimer;
    int times;

    //setting
    clientSetting setting;
};
#endif // MAINWINDOW_H
