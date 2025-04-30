#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::loginButtonClicked);
    connect(ui->exitButton, &QPushButton::clicked, this, &LoginWindow::exitButtonClicked);
    connect(ui->loginLine, &QLineEdit::editingFinished, this, &LoginWindow::loginLineEditingFinished);
    connect(ui->passwordLine, &QLineEdit::editingFinished, this, &LoginWindow::passwordLineEditingFinished);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::loginButtonClicked()
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


void LoginWindow::exitButtonClicked()
{
    this->close();
}


void LoginWindow::loginLineEditingFinished()
{
    ui->passwordLine->setFocus();
}


void LoginWindow::passwordLineEditingFinished()
{
    loginButtonClicked();
}

