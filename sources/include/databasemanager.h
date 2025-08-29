#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include "treechangetracker.h"
#include "material.h"
#include "extramaterialoption.h"

#include <QString>
#include <QSqlQuery>

class DatabaseManager
{
public:
    DatabaseManager();

    QString lastError() const {
        return error;
    }

    void setupDatabaseConnection(QSqlDatabase &database, const QString &connectionName, const QString &configFilePath);

signals:
    void errorOccurred(const QString &title, const QString &error);

private:
    QString error;

    QMap<QString, QString> parseDatabaseConnectionConfig(const QString &filePath);
};

namespace CRUD {
void selectUserAccess(QSqlQuery &query, const QString &login, const QString &password);
void selectMaterialCategories(QSqlQuery &query);
void selectWorkCategories(QSqlQuery &query);
void selectMaterialTypesByCategory(QSqlQuery &query, QString categoryName);
void selectMaterialsByType(QSqlQuery &query, const QString &typeName);
void selectMaterialsByCategory(QSqlQuery &query, const QString &categoryName);
void selectMaterialsByName(QSqlQuery &query, const QString &name);
void selectExtraMaterialOptions(QSqlQuery &query, const int &materialId);

void insertCategory(QSqlQuery &query, const TreeChange &category);
void insertType(QSqlQuery &query, const TreeChange &type);
void insertMaterial(QSqlQuery &query, const QString &typeName, const Material &material);
void insertExtraMaterialOption(QSqlQuery &query, const int &materialId, const ExtraMaterialOption &option);

void deleteCategory(QSqlQuery &query, const TreeChange &category);
void deleteType(QSqlQuery &query, const TreeChange &type);
void deleteMaterial(QSqlQuery &query, const int &id);
void deleteExtraMaterialOption(QSqlQuery &query, const int &id);
}
#endif // DATABASEMANAGER_H
