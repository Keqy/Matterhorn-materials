#include "include/databasemanager.h"

#include <QFile>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>

namespace CRUD {
// --SELECT.
void selectUserAccessByAuthKey(QSqlQuery &query, const QString &authKey)
{
    query.prepare("SELECT access_level FROM users WHERE auth_key=crypt(?, auth_key)");
    query.addBindValue(authKey);
    query.exec();
}

void selectUserAccess(QSqlQuery &query, const QString &login, const QString &password)
{
    query.prepare("SELECT access_level FROM users WHERE login=? AND password=crypt(?, password);");
    query.addBindValue(login);
    query.addBindValue(password);
    query.exec();
}

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
                  "m.id, "
                  "m.name, "
                  "m.measure, "
                  "m.cost_price, "
                  "m.min_amount, "
                  "m.weight, "
                  "m.waste_rate, "
                  "to_char(m.last_edit_date, 'DD.MM.YYYY') "
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
                  "m.id, "
                  "m.name, "
                  "m.measure, "
                  "m.cost_price, "
                  "m.min_amount, "
                  "m.weight, "
                  "m.waste_rate, "
                  "to_char(m.last_edit_date, 'DD.MM.YYYY') "
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
    query.prepare("SELECT "
                  "m.id, "
                  "m.name, "
                  "m.measure, "
                  "m.cost_price, "
                  "m.min_amount, "
                  "m.weight, "
                  "m.waste_rate, "
                  "to_char(m.last_edit_date, 'DD.MM.YYYY') "
                  "FROM materials m "
                  "WHERE m.name ILIKE ?;");
    query.addBindValue("%" + name + "%");
    query.exec();
}

void selectExtraMaterialOptions(QSqlQuery &query, const int &materialId)
{
    query.prepare("SELECT id, name, value, measure FROM extra_material_options WHERE material_id = ?;");
    query.addBindValue(materialId);
    query.exec();
}

// --INSERT.
void insertUserAuthKey(QSqlQuery &query, const QString &login, const QString &authKey)
{
    query.prepare("UPDATE users SET auth_key=crypt(?, gen_salt('md5')) WHERE login=?;");
    query.addBindValue(authKey);
    query.addBindValue(login);
    query.exec();
}

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
                    "waste_rate, "
                    "last_edit_date) "
                  "VALUES("
                    "(SELECT id FROM material_types WHERE name = ?), "
                    "?, ?, ?, ?, ?, ?, ?);");
    query.addBindValue(typeName);
    query.addBindValue(material.name);
    query.addBindValue(material.measure);
    query.addBindValue(material.costPrice);
    query.addBindValue(material.minAmount);
    query.addBindValue(material.weight);
    query.addBindValue(material.wasteRate);
    query.addBindValue(material.date);
    query.exec();
}

void insertExtraMaterialOption(QSqlQuery &query, const int &materialId, const ExtraMaterialOption &option)
{
    query.prepare("INSERT INTO "
                  "extra_material_options("
                    "material_id, "
                    "name, "
                    "value, "
                    "measure) "
                  "VALUES(?, ?, ?, ?);");
    query.addBindValue(materialId);
    query.addBindValue(option.name);
    query.addBindValue(option.value);
    query.addBindValue(option.measure);
    query.exec();
}

// --DELETE.
void deleteUserAuthKey(QSqlQuery &query, const QString &authKey)
{
    query.prepare("UPDATE users "
                  "SET auth_key=NULL "
                  "WHERE auth_key=crypt(?, auth_key)");
    query.addBindValue(authKey);
    query.exec();
}
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

void deleteMaterial(QSqlQuery &query, const int &id)
{
    query.prepare("DELETE FROM materials WHERE id = ?;");
    query.addBindValue(id);
    query.exec();
}

void deleteExtraMaterialOption(QSqlQuery &query, const int &id)
{
    query.prepare("DELETE FROM extra_material_options WHERE id = ?;");
    query.addBindValue(id);
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
