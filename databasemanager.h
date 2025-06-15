#include <QString>
#include <QSqlQuery>

#include "treechangetracker.h"

#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

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
void selectMaterialCategories(QSqlQuery &query);
void selectWorkCategories(QSqlQuery &query);
void selectMaterialTypesByCategory(QSqlQuery &query, QString categoryName);
void selectMaterialsByType(QSqlQuery &query, const QString &typeName);
void selectMaterialsByCategory(QSqlQuery &query, const QString &categoryName);
void selectMaterialsByName(QSqlQuery &query, const QString &name);

void insertCategory(QSqlQuery &query, const TreeChange &category);
}
#endif // DATABASEMANAGER_H
