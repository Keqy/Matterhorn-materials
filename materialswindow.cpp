#include "materialswindow.h"
#include "database_utils.h"
#include "ui_materialswindow.h"

#include <QMessageBox>

MaterialsWindow::MaterialsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MaterialsWindow)
{
    ui->setupUi(this);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", "materials_connection");
    QMap<QString, QString> dbConfig;
    try {
        dbConfig = parseDatabaseConnectionConfig("config/db.config");
    } catch (QString &errorText) {
        QMessageBox::critical(this, "Ошибка конфигурации db.config", errorText);
        throw std::runtime_error(errorText.toStdString());
    }
    db.setHostName(dbConfig["HOST"]);
    db.setDatabaseName(dbConfig["DBNAME"]);
    db.setUserName(dbConfig["USERNAME"]);
    db.setPassword(dbConfig["PASSWORD"]);
    db.open();

    QSqlQuery query(db);
    CRUD::selectMaterialCategories(query);

    QTreeWidgetItem *allMaterials = new QTreeWidgetItem();
    allMaterials->setText(0, "Все материалы");
    allMaterials->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));

    QList<QTreeWidgetItem *> items;
    while (query.next()) {
        items.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                                         query.value(0).toStringList()));
    }

    ui->materialsTreeWidget->insertTopLevelItem(0, allMaterials);
    for (QTreeWidgetItem *item : items) {
        allMaterials->addChild(item);
    }
}
MaterialsWindow::~MaterialsWindow()
{
    delete ui;
    db.removeDatabase("materials_connection");
}
