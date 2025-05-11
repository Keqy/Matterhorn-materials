#include "materialswindow.h"
#include "databasemanager.h"
#include "ui_materialswindow.h"

#include <QMessageBox>
#include <QSqlError>

MaterialsWindow::MaterialsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MaterialsWindow)
{
    ui->setupUi(this);

    QSqlDatabase db;
    DatabaseManager dbManager;
    dbManager.setupDatabaseConnection(db, "materials_connection", "config/db.config");
    if (!dbManager.lastError().isEmpty()) {
        QMessageBox::critical(this, "Ошибка соединения с БД", dbManager.lastError());
    }

    updateMaterialsTree();
}

MaterialsWindow::~MaterialsWindow()
{
    delete ui;
    db.removeDatabase("materials_connection");
}

void MaterialsWindow::updateMaterialsTree()
{
    QSqlDatabase db = QSqlDatabase::database("materials_connection");
    QSqlQuery query(db);
    CRUD::selectMaterialCategories(query);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
        return;
    }

    QTreeWidgetItem *allMaterialsTreeItem = new QTreeWidgetItem();
    allMaterialsTreeItem->setText(0, "Все материалы");
    allMaterialsTreeItem->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
    ui->materialsTreeWidget->insertTopLevelItem(0, allMaterialsTreeItem);

    QList<QTreeWidgetItem *> items;
    while (query.next()) {
        items.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                                         query.value(0).toStringList()));
    }


    for (QTreeWidgetItem *item : items) {
        allMaterialsTreeItem->addChild(item);
    }

    QTreeWidgetItem *allJobsTreeItem = new QTreeWidgetItem();
    allJobsTreeItem->setText(0, "Все работы");
    allJobsTreeItem->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties));
    ui->materialsTreeWidget->insertTopLevelItem(1, allJobsTreeItem);
}
