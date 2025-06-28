#include "include/logindialog.h"
#include "include/mainwindow.h"

#include <QApplication>
#include <QEventLoop>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LoginDialog loginDialog;
    if (!loginDialog.exec()) {
        return 0x00001;
    }

    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}
