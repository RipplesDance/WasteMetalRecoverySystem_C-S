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
#include <QDesktopServices>
#include <QUrl>
#include"batteryMaterialConcentration.h"
#include"quotation.h"
#include"transaction.h"
#include"transactionHistoryDialog.h"
#include"metalPrice.h"
#include"recoveryCost.h"
#include"settingDialog.h"
#include"clientSetting.h"
#include"addressDialog.h"
#include"address.h"
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
    double fetchNumberFromString(QString str);//get all numbers from a Qstring type
    void makeDirPath(QString filePath); // create certain file path
    void sellButtonClicked(QString sellingWay); // 2 sell buttons clicked
    void frameClicked(QString frameType); // personal data area clicked
    void updateTransaction(transaction data); // update when status changes
    void newTransaction(transaction data); // new transaction will send msg to server
    void sendMsgToServer(int type, transaction data); // communicate with server on way 1
    void sendMsgToServer(int type); // communicate with server on way 2
    void updateMetalPrice(metalPrice data);// trigger when server send new metal price
    void updateTypeComboBox(); // trigger when change current item
    bool clearDir(QString dirPath); // remove certain dir path
    void socketConnectToServer(); // to connect remote server
    QString getUUID(); // grab the only uuid on device
    void resizeWindow(); // modify the whole window size
    void startHandshake(); // trigger when connect to server successfully
    bool dirPathChanged(QString oldPath, QString newPath); // change from old dir path to new dir path

    //price card redirect to extral link
    void openKLine();

    //polish
    void polishInterface(); // general polish function
    void setupCardShadow(QWidget *card); // card shadow

protected:
    void closeEvent(QCloseEvent *event) override; // double check close
    bool eventFilter(QObject *watched, QEvent *event) override; // only for price card to redirect to extral link

public slots:
    void onSlideValueChanged(int value); //trigger if SOH bar value changed
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
    addressDialog* address_dialog;

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
