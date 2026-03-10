#ifndef ONLINECLIENTSDIALOG_H
#define ONLINECLIENTSDIALOG_H

#include <QDialog>
#include"clientInfo.h"

namespace Ui {
class onlineClientsDialog;
}

class onlineClientsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit onlineClientsDialog(QWidget *parent = nullptr);
    ~onlineClientsDialog();
    void addClient(clientInfo* data);
    void updateClientNumber(int n);
    void clearTreeWidget();
    void init();

    int sequence;

signals:
    void heartBeat();
private:
    Ui::onlineClientsDialog *ui;
};

#endif // ONLINECLIENTSDIALOG_H
