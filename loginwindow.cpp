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
    QObject::connect(ui->loginLine, &QLineEdit::returnPressed, this, &LoginWindow::setPasswordLineFocus);
    QObject::connect(ui->passwordLine, &QLineEdit::returnPressed, this, &LoginWindow::setLoginButtonFocus);

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

QMap<QString, QString> LoginWindow::parseDatabaseConfig(const QString &filePath)
{
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Ошибка открытия файла bd.config", configFile.errorString());
        throw std::runtime_error("Ошибка открытия файла bd.config — " + configFile.errorString().toStdString());
    }

    QMap<QString, QString> dbConfig;
    static const QRegularExpression regex(R"(^\s*([A-Z_]+)\s*=\s*(.+?)\s*$)"); // Регулярное выражение формата KEY=VALUE.
    QRegularExpressionMatch match;

    QStringList errorLines;
    const QStringList configLines = QString(configFile.readAll()).split("\n");
    for (const QString &line : configLines) {
        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }

        // Поиск параметров по соответствию формату regex.
        match = regex.match(line);
        if (match.hasMatch()) {
            QString key = match.captured(1);
            QString value = match.captured(2);
            dbConfig.insert(key, value);
        } else {
            // Неккоректные строки.
            errorLines << line;
        }
    }
    if (!errorLines.isEmpty()) {
        QMessageBox::warning(this,
                             "Предупреждение .config",
                             "Некорректные строки в .config:\n" + errorLines.join("\n"));
    }
    configFile.close();

    // Проверка наличия необходимых параметров в спаршеном конфиге.
    QStringList missingOptions;
    const QStringList requiredOptions = {"HOST", "DBNAME", "USERNAME", "PASSWORD"};
    for (const QString &option : requiredOptions) {
        if (!dbConfig.contains(option)) {
            missingOptions << option;
        }
    }
    if (!missingOptions.isEmpty()) {
        QMessageBox::critical(this,
                              "Ошибка конфигурации",
                              "Отсутствует параметр:\n" + missingOptions.join("\n"));
        throw std::invalid_argument("Отсутсвуют параметры конфигурации\n"
                                    + missingOptions.join("\n").toStdString());
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
