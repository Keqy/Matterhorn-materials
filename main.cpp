#include "loginwindow.h"
#include "mainwindow.h"

#include <QApplication>
#include <QEventLoop>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow *loginWindow = new LoginWindow;
    QEventLoop *eventLoop = new QEventLoop;
    QObject::connect(loginWindow, &LoginWindow::login_signal, eventLoop, &QEventLoop::quit);

    loginWindow->show();
    eventLoop->exec();

    delete loginWindow; loginWindow = nullptr;
    delete eventLoop; eventLoop = nullptr;
    // TODO: Сделать так, чтобы eventLoop закрывался в случае принудительного закрытия, а не висел процессом.

    MainWindow mainWindow;
    mainWindow.show();
    return a.exec();
}
