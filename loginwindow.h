#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QSqlDatabase>
#include <QWidget>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

signals:
    void login_signal();

private:
    Ui::LoginWindow *ui;
    QSqlDatabase db;

private slots:
    void login();
    void exit();
    void setLoginButtonFocus();
    void setPasswordLineFocus();
};

#endif // LOGINWINDOW_H
