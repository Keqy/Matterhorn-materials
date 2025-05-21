#include "databasemanager.h"

#include <QFile>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

namespace CRUD {
void selectMaterialCategories(QSqlQuery &query)
{
    query.exec("SELECT name FROM material_categories;");
}

void selectWorkCategories(QSqlQuery &query)
{
    query.exec("SELECT name FROM work_categories;");
}

void selectMaterialTypes(QSqlQuery &query)
{
    query.exec("SELECT "
               "t.name, "
               "m.name, "
               "m.measure, "
               "m.cost_price, "
               "m.min_amount, "
               "m.weight, "
               "m.waste_rate "
               "FROM "
               "materials m "
               "JOIN "
               "material_categories c "
               "ON m.category_id = c.id;");
}

void selectMaterialsByName(QSqlQuery &query, const QString &name)
{
    query.prepare("SELECT name, measure, cost_price, min_amount, weight, waste_rate FROM materials WHERE name = ?;");
    query.addBindValue(name);
    query.exec();
}
} // namespace CRUD

DatabaseManager::DatabaseManager() {}

QMap<QString, QString> DatabaseManager::parseDatabaseConnectionConfig(const QString &filePath)
{
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error = configFile.errorString();
        throw std::invalid_argument(("Не удалось открыть конфиг " + filePath).toStdString());
    }

    // Regular expression "KEY=VALUE".
    static const QRegularExpression regex(R"(^\s*([A-Z_]+)\s*=\s*(.+?)\s*$)");
    QRegularExpressionMatch match;

    QMap<QString, QString> dbConfig;
    const QStringList configLines = QString(configFile.readAll()).split("\n");
    for (const QString &line : configLines) {
        if (line.isEmpty() || line.startsWith("#")) {
            continue;
        }

        // Searches for a regular expression match.
        match = regex.match(line);
        if (match.hasMatch()) {
            QString key = match.captured(1);
            QString value = match.captured(2);
            dbConfig.insert(key, value);
        }
    }
    configFile.close();

    // Check if the required parameters are present in the parsed config.
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
        //TODO: log std::runtime_error(error.toStdString());
        return;
    } else {
        database = QSqlDatabase::addDatabase(dbConfig["DRIVER"], connectionName);
        database.setHostName(dbConfig["HOST"]);
        database.setDatabaseName(dbConfig["DBNAME"]);
        database.setUserName(dbConfig["USERNAME"]);
        database.setPassword(dbConfig["PASSWORD"]);
    }
}
