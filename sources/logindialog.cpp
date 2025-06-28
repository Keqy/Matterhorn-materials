#include "include/logindialog.h"
#include "ui_logindialog.h"

#include <QMessageBox>
#include <QSqlError>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->loginButton, &QPushButton::clicked, this, &LoginDialog::login);
    QObject::connect(ui->exitButton, &QPushButton::clicked, this, &LoginDialog::exit);
    QObject::connect(ui->loginLine, &QLineEdit::returnPressed, this, &LoginDialog::setPasswordLineFocus);
    QObject::connect(ui->passwordLine, &QLineEdit::returnPressed, this, &LoginDialog::login);

    QSqlDatabase db;
    DatabaseManager dbManager;
    dbManager.setupDatabaseConnection(db, "login_connection", "config/db.config");
    if (!dbManager.lastError().isEmpty()) {
        QMessageBox::critical(this, "Ошибка соединения с БД", dbManager.lastError());
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
    db.removeDatabase("login_connection");
}

void LoginDialog::login()
{
    QSqlDatabase db = QSqlDatabase::database("login_connection");
    QSqlQuery query(db);

    QString login = ui->loginLine->text();
    QString password = ui->passwordLine->text();

    //TODO: Перенести это в CRUD.
    // if (!db.open()) {
    //     QMessageBox::critical(this, "Ошибка базы данных", db.lastError().text());
    // } else {
    //     query.prepare("SELECT id FROM users WHERE login=? AND password=crypt(?, password);");
    //     query.addBindValue(login);
    //     query.addBindValue(password);
    //     query.exec();
    // }
    CRUD::selectUserAccess(query, login, password);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса базы данных", query.lastError().text());
        return;
    } else {
        if (query.first()) {
            userAccessLevel = query.value(0).toInt();
            LoginDialog::accept();
        } else {
            ui->errorLabel->setText("Неправильный логин или пароль");
            ui->passwordLine->clear();
        }
    }
}

void LoginDialog::setPasswordLineFocus()
{
    ui->passwordLine->setFocus();
}

void LoginDialog::exit()
{
    this->close();
}
