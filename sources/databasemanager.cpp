#include "include/databasemanager.h"

#include <QFile>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

namespace CRUD {
// --SELECT.
void selectMaterialCategories(QSqlQuery &query)
{
    query.exec("SELECT name "
               "FROM material_categories;");
}

void selectWorkCategories(QSqlQuery &query)
{
    query.exec("SELECT name "
               "FROM work_categories;");
}

void selectMaterialTypesByCategory(QSqlQuery &query, QString categoryName)
{
    query.prepare("SELECT t.name "
                  "FROM material_types t "
                  "JOIN material_categories c "
                  "ON t.category_id = c.id "
                  "WHERE c.name = ?");
    query.addBindValue(categoryName);
    query.exec();
}

void selectMaterialsByType(QSqlQuery &query, const QString &typeName)
{
    query.prepare("SELECT "
                  "m.name, "
                  "m.measure, "
                  "m.cost_price, "
                  "m.min_amount, "
                  "m.weight, "
                  "m.waste_rate "
                  "FROM materials m "
                  "JOIN material_types t "
                  "ON m.type_id = t.id "
                  "WHERE t.name = ?;");
    query.addBindValue(typeName);
    query.exec();
}

void selectMaterialsByCategory(QSqlQuery &query, const QString &categoryName)
{
    query.prepare("SELECT "
                  "m.name, "
                  "m.measure, "
                  "m.cost_price, "
                  "m.min_amount, "
                  "m.weight, "
                  "m.waste_rate "
                  "FROM materials m "
                  "JOIN material_types t "
                  "ON m.type_id = t.id "
                  "JOIN material_categories c "
                  "ON t.category_id = c.id "
                  "WHERE c.name = ?;");
    query.addBindValue(categoryName);
    query.exec();
}

void selectMaterialsByName(QSqlQuery &query, const QString &name)
{
    query.exec("SELECT "
               "m.name, "
               "m.measure, "
               "m.cost_price, "
               "m.min_amount, "
               "m.weight, "
               "m.waste_rate "
               "FROM materials m;");
}

// --INSERT.
void insertCategory(QSqlQuery &query, const TreeChange &category)
{
    query.prepare("INSERT INTO "
                  "material_categories(name) "
                  "VALUES (?);");
    query.addBindValue(category.name);
    query.exec();
}

void insertType(QSqlQuery &query, const TreeChange &type)
{
    query.prepare("INSERT INTO "
                  "material_types(category_id, name) "
                  "VALUES("
                  "(SELECT id FROM material_categories WHERE name = ?), "
                  "?);");
    query.addBindValue(type.parentName);
    query.addBindValue(type.name);
    query.exec();
}

void insertMaterial(QSqlQuery &query, const QString &typeName, const Material &material)
{
    query.prepare("INSERT INTO "
                  "materials("
                    "type_id, "
                    "name, "
                    "measure, "
                    "cost_price, "
                    "min_amount, "
                    "weight, "
                    "waste_rate) "
                  "VALUES("
                    "(SELECT id FROM material_types WHERE name = ?), "
                    "?, ?, ?, ?, ?, ?)");
    query.addBindValue(typeName);
    query.addBindValue(material.name);
    query.addBindValue(material.measure);
    query.addBindValue(material.costPrice);
    query.addBindValue(material.minAmount);
    query.addBindValue(material.weight);
    query.addBindValue(material.wasteRate);
    query.exec();
}

// --DELETE.
void deleteCategory(QSqlQuery &query, const TreeChange &category)
{
    query.prepare("DELETE FROM "
                  "material_categories "
                  "WHERE "
                  "name = ?;");
    query.addBindValue(category.name);
    query.exec();
}

void deleteType(QSqlQuery &query, const TreeChange &type)
{
    query.prepare("DELETE FROM "
                  "material_types "
                  "WHERE "
                  "name = ?;");
    query.addBindValue(type.name);
    query.exec();
}
} // namespace CRUD

DatabaseManager::DatabaseManager() {}

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
