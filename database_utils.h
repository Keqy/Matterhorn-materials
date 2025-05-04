#include <QSqlQuery>

#ifndef DATABASE_UTILS_H
#define DATABASE_UTILS_H

QMap<QString, QString> parseDatabaseConnectionConfig(const QString &filePath);

namespace CRUD {
void selectMaterialCategories(QSqlQuery &query);

};
#endif // DATABASE_UTILS_H
