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
    int accessLevel = loginDialog.userAccessLevel;

    MainWindow mainWindow;
    mainWindow.showWithAccess(accessLevel);
    return a.exec();
}
