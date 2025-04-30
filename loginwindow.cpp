#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::login);
    connect(ui->exitButton, &QPushButton::clicked, this, &LoginWindow::exit);
    connect(ui->loginLine, &QLineEdit::editingFinished, this, &LoginWindow::setPasswordLineFocus);
    connect(ui->passwordLine, &QLineEdit::editingFinished, this, &LoginWindow::setLoginButtonFocus);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::login()
{
    QString login = ui->loginLine->text();
    QString password = ui->passwordLine->text();

    if (login == "admin" & password == "admin") {
        this->close();
    }
    else {
        ui->errorLabel->setText("Неверный логин или пароль");
        ui->passwordLine->clear();
    }
}


void LoginWindow::exit()
{
    this->close();
}


void LoginWindow::setPasswordLineFocus()
{
    ui->passwordLine->setFocus();
}


void LoginWindow::setLoginButtonFocus()
{
    login();
}

