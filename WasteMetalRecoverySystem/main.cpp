#include <QFile>
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file(":/QSS/MainWindow.qss");
        if (file.open(QFile::ReadOnly)) {
            QString styleSheet = QLatin1String(file.readAll());
            a.setStyleSheet(styleSheet);
            file.close();
        }

    MainWindow w;
    w.setWindowIcon(QIcon(":/images/res/battery_recycling.ico"));
    w.show();
    return a.exec();
}
