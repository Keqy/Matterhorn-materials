#include "loginwindow.h"
#include "mainwindow.h"

#include <QApplication>
#include <QEventLoop>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow loginWindow;
    MainWindow mainWindow;
    //loginWindow.show();
    mainWindow.show();
    return a.exec();
}
