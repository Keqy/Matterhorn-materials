#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "databasemanager.h"

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    int userAccessLevel;

private:
    Ui::LoginDialog *ui;
    QSqlDatabase db;
    DatabaseManager dbManager;

    void login();
    void exit();
    void setLoginButtonFocus();
    void setPasswordLineFocus();
};

#endif // LOGINDIALOG_H
