#include "databasemanager.h"

#include <QFile>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

DatabaseManager::DatabaseManager() {}

QMap<QString, QString> DatabaseManager::parseDatabaseConnectionConfig(const QString &filePath)
{
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error = configFile.errorString();
        throw std::invalid_argument(("Не удалось открыть конфиг " + filePath).toStdString());
    }

    // Регулярное выражение формата KEY=VALUE.
    static const QRegularExpression regex(R"(^\s*([A-Z_]+)\s*=\s*(.+?)\s*$)");
    QRegularExpressionMatch match;

    QMap<QString, QString> dbConfig;
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
        }
    }
    configFile.close();

    // Проверка наличия необходимых параметров в спаршеном конфиге.
    QStringList missingOptions;
    const QStringList requiredOptions = {"DRIVER", "HOST", "DBNAME", "USERNAME", "PASSWORD"};
    for (const QString &option : requiredOptions) {
        if (!dbConfig.contains(option)) {
            missingOptions << option;
        }
    }
    if (!missingOptions.isEmpty()) {
        error = QString("В config отсутствует параметр:\n" + missingOptions.join("\n"));
    }

    return dbConfig;
}

void DatabaseManager::setupDatabaseConnection(QSqlDatabase &database, const QString &connectionName, const QString &configFilePath)
{
    QMap<QString, QString> dbConfig = parseDatabaseConnectionConfig(configFilePath);
    if (!error.isEmpty()) {
        //TODO: лог std::runtime_error(error.toStdString());
        return;
    } else {
        database = QSqlDatabase::addDatabase(dbConfig["DRIVER"], connectionName);
        database.setHostName(dbConfig["HOST"]);
        database.setDatabaseName(dbConfig["DBNAME"]);
        database.setUserName(dbConfig["USERNAME"]);
        database.setPassword(dbConfig["PASSWORD"]);
    }
}

namespace CRUD {
void selectMaterialCategories(QSqlQuery &query)
{
    query.exec("SELECT name FROM material_categories;");
}
} // namespace CRUD
