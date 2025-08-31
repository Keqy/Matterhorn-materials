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

    authKey = qgetenv("USERNAME") + qgetenv("COMPUTERNAME");
    db = QSqlDatabase::database("login_connection");
    QSqlQuery query(db);
    CRUD::selectUserAccessByAuthKey(query, authKey);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса базы данных", query.lastError().text());
        return;
    }
    if (query.first()) {
        ui->loginLine->setText("************");
        ui->passwordLine->setText("************");
        isAuthKeyValid = true;
        userAccessLevel = query.value(0).toInt();
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
    if (login == "************" && password == "************" && isAuthKeyValid) {
        LoginDialog::accept();
        return;
    }

    CRUD::selectUserAccess(query, login, password);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса базы данных", query.lastError().text());
        return;
    } else if (query.first()) {
        userAccessLevel = query.value(0).toInt();
        if (ui->rememberMeCheckBox->isChecked() && login != "************") {
            if (!db.transaction()) {
                QMessageBox::critical(this, "Не удалось начать транзакцию в базу данных", db.lastError().text());
            }
            CRUD::deleteUserAuthKey(query, authKey);
            CRUD::insertUserAuthKey(query, login, authKey);
            if (!db.commit()) {
                QMessageBox::critical(this, "Не удалось завершить транзакцию в базу данных", db.lastError().text());
            }
        }
        LoginDialog::accept();
    } else {
        ui->errorLabel->setText("Неправильный логин или пароль");
        ui->passwordLine->clear();
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
