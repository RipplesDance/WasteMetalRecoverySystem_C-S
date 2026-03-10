#ifndef TRANSACTIONHISTORYDIALOG_H
#define TRANSACTIONHISTORYDIALOG_H

#include <QDialog>
#include<QtDebug>
#include<QVector>
#include<QFile>
#include<QDir>
#include<QDataStream>
#include<QMessageBox>
#include<QListWidget>
#include<QListWidgetItem>
#include<QMenu>
#include<QAction>
#include"transaction.h"

namespace Ui {
class transactionHistoryDialog;
}

class transactionHistoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit transactionHistoryDialog(QWidget *parent = nullptr);
    ~transactionHistoryDialog();

    void init();
    void updataTransaction(transaction data);
    void updataListWidget();

public slots:
    void onNewTransaction();
    void selectedItem(QListWidgetItem *item);
    void sortBoxChanged(QString way);
    void showTransactionContextMenu(const QPoint &pos);
public:
    QString filePath;

private:
    Ui::transactionHistoryDialog *ui;
    QVector<transaction> fileVector;
};

#endif // TRANSACTIONHISTORYDIALOG_H
