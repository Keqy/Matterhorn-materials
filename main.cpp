#include "loginwindow.h"
#include "mainwindow.h"

#include <QApplication>
#include <QEventLoop>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow *loginWindow = new LoginWindow;
    QEventLoop *eventLoop = new QEventLoop;
    QObject::connect(loginWindow, &LoginWindow::loginSignal, eventLoop, &QEventLoop::quit);

    loginWindow->show();
    eventLoop->exec();

    delete loginWindow;
    delete eventLoop;
    loginWindow = nullptr;
    eventLoop = nullptr;

    // TODO: Fix process freezing on early exit without login. Refactor LoginWindow to LoginDialog.

    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}
