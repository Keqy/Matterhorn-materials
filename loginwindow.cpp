#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::login);
    QObject::connect(ui->exitButton, &QPushButton::clicked, this, &LoginWindow::exit);
    QObject::connect(ui->loginLine, &QLineEdit::editingFinished, this, &LoginWindow::setPasswordLineFocus);
    QObject::connect(ui->passwordLine, &QLineEdit::editingFinished, this, &LoginWindow::setLoginButtonFocus);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "login_connection");
    db.setHostName("???");
    db.setDatabaseName("???");
    db.setUserName("???");
    db.setPassword("???");
    // TODO: Спрятать данные для входа в конфиг.
}

LoginWindow::~LoginWindow()
{
    delete ui;
    // Удаляется соедениние.
    db.removeDatabase("login_connection");
}

void LoginWindow::login()
{
    QSqlDatabase db = QSqlDatabase::database("login_connection");
    QSqlQuery query(db);

    QString login = ui->loginLine->text();
    QString password = ui->passwordLine->text();

    if (!db.open()) {
        QMessageBox::critical(this, "Ошибка базы данных", db.lastError().text());
    } else {
        query.prepare("SELECT id FROM users WHERE login=? AND password=crypt(?, password);");
        query.addBindValue(login);
        query.addBindValue(password);
        query.exec();
    }

    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса базы данных", query.lastError().text());
    } else {
        if (query.first())
            emit login_signal();
        else
            ui->errorLabel->setText("Неправильный логин или пароль");
    }
}

// Меняется фокус на passwordLine по нажатию Enter в loginLine.
void LoginWindow::setPasswordLineFocus()
{
    ui->passwordLine->setFocus();
}

// Менятся фокус на loginButton по нажатию Enter в passwordLine.
void LoginWindow::setLoginButtonFocus()
{
    login();
}

void LoginWindow::exit()
{
    this->close();
}
