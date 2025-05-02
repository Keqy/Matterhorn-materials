#include "loginwindow.h"
#include "ui_loginwindow.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QRegularExpression>
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
    QMap<QString, QString> config = parseDatabaseConfig("config/bd.config");
    db.setHostName(config["HOST"]);
    db.setDatabaseName(config["DBNAME"]);
    db.setUserName(config["USERNAME"]);
    db.setPassword(config["PASSWORD"]);
}

LoginWindow::~LoginWindow()
{
    delete ui;
    // Удаляется соедениние.
    db.removeDatabase("login_connection");
}

// TODO: Провести рефакторинг стилистики кода этой функции.
QMap<QString, QString> LoginWindow::parseDatabaseConfig(const QString &file)
{
    QFile configFile(file);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка открытия файла bd.config", configFile.errorString());
        throw;
    }

    QMap<QString, QString> dbConfig;
    // Регулярное выражение формата KEY=VALUE.
    static QRegularExpression regex(R"(^\s*([A-Z_]+)\s*=\s*(.+?)\s*$)");
    QRegularExpressionMatch match;

    QStringList errorLines; // Список неккоретных строк.
    const QStringList connectionData = QString(configFile.readAll()).split("\n");
    for (const QString &line : connectionData) {
        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }

        // Поиск соответствия формату "KEY=VALUE".
        match = regex.match(line);
        if (match.hasMatch()) {
            QString key = match.captured(1);
            QString value = match.captured(2);
            dbConfig.insert(key, value);
        } else {
            // Собирает некорректные строки.
            errorLines << line;
        }
    }
    // Предупреждение о неккоректных строках.
    if (!errorLines.isEmpty()) {
        QMessageBox::warning(this,
                             "Предупреждение .config",
                             "Некорректные строки в .config:\n" + errorLines.join("\n"));
    }
    configFile.close();

    // Проверка наличия всех параметров в спаршеном конфиге.
    QStringList missingOptions;
    const QStringList requiredOptions = {"HOST", "DBNAME", "USERNAME", "PASSWORD"};
    for (const QString &option : requiredOptions) {
        if (!dbConfig.contains(option)) {
            missingOptions << option;
        }
    }
    // Отсутствие параметров является критической ошибкой. Невозможно установить соединение с БД.
    if (!missingOptions.isEmpty()) {
        QMessageBox::critical(this,
                              "Ошибка конфигурации",
                              "Отсутствует параметр:\n" + missingOptions.join("\n"));
        throw;
    }

    return dbConfig;
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
            emit login_signal(); // Сигнал на окончание EventLoop в main.cpp. Открытие главного окна.
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
