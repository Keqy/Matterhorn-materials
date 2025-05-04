#include "loginwindow.h"
#include "database_utils.h"
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
    QObject::connect(ui->loginLine, &QLineEdit::returnPressed, this, &LoginWindow::setPasswordLineFocus);
    QObject::connect(ui->passwordLine, &QLineEdit::returnPressed, this, &LoginWindow::setLoginButtonFocus);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "login_connection");
    QMap<QString, QString> dbConfig;
    try {
        dbConfig = parseDatabaseConnectionConfig("config/db.config");
    } catch (QString &errorText) {
        QMessageBox::critical(this, "Ошибка конфигурации db.config", errorText);
        throw std::runtime_error(errorText.toStdString());
    }

    db.setHostName(dbConfig["HOST"]);
    db.setDatabaseName(dbConfig["DBNAME"]);
    db.setUserName(dbConfig["USERNAME"]);
    db.setPassword(dbConfig["PASSWORD"]);
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

    //TODO: Перенести это в CRUD.
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
            // Сигнал на окончание EventLoop в main.cpp. Открытие главного окна.
            emit login_signal();
        else {
            ui->errorLabel->setText("Неправильный логин или пароль");
            ui->passwordLine->clear();
        }
    }
}

// Меняется фокус по нажатию Enter в loginLine на passwordLine.
void LoginWindow::setPasswordLineFocus()
{
    ui->passwordLine->setFocus();
}

// Менятся фокус по нажатию Enter в passwordLine на loginButton.
void LoginWindow::setLoginButtonFocus()
{
    login();
}

void LoginWindow::exit()
{
    this->close();
}
