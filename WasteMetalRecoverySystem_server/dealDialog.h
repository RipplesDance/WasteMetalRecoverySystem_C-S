#ifndef DEALDIALOG_H
#define DEALDIALOG_H

#include <QDialog>
#include<QDebug>
#include<QMessageBox>
#include"transaction.h"

namespace Ui {
class dealDialog;
}

class dealDialog : public QDialog
{
    Q_OBJECT

public:
    explicit dealDialog(QWidget *parent = nullptr);
    ~dealDialog();
    void updateTransaction(transaction data);
    void disableBtn();
    void enableBtn();

signals:
    void finished(bool isAccept);

private:
    Ui::dealDialog *ui;
};

#endif // DEALDIALOG_H
