#ifndef METALPRICEDIALOG_H
#define METALPRICEDIALOG_H

#include <QDialog>
#include"metalPrice.h"

namespace Ui {
class metalPriceDialog;
}

class metalPriceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit metalPriceDialog(QWidget *parent = nullptr);
    ~metalPriceDialog();
    void init();
    void setMetalPrice(metalPrice data);

signals:
    void update(metalPrice data);

private:
    Ui::metalPriceDialog *ui;
};

#endif // METALPRICEDIALOG_H
