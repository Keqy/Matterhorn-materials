#include "database_utils.h"

#include <QFile>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSqlError>
#include <QSqlQuery>

QMap<QString, QString> parseDatabaseConnectionConfig(const QString &filePath)
{
    QFile configFile(filePath);
    if (!configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw configFile.errorString();
    }

    QMap<QString, QString> dbConfig;
    // Регулярное выражение формата KEY=VALUE.
    static const QRegularExpression regex(R"(^\s*([A-Z_]+)\s*=\s*(.+?)\s*$)");
    QRegularExpressionMatch match;

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
    const QStringList requiredOptions = {"HOST", "DBNAME", "USERNAME", "PASSWORD"};
    for (const QString &option : requiredOptions) {
        if (!dbConfig.contains(option)) {
            missingOptions << option;
        }
    }
    if (!missingOptions.isEmpty()) {
        throw QString("Отсутствует параметр:\n" + missingOptions.join("\n"));
    }

    return dbConfig;
}

namespace CRUD {
void selectMaterialCategories(QSqlQuery &query)
{
    query.exec("SELECT name FROM material_categories");

    if (query.lastError().isValid()) {
        QMessageBox::critical(nullptr, "Ошибка запроса к БД", query.lastError().text());
    }
}
} // namespace CRUD
