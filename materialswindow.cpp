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


    setMaterialsTableColumnWidth();
    setMaterialsExtraOptionsTableColumnWidth();
    setMaterialWorkAppropriatenessTableColumnWidth();
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

    QTreeWidgetItem *rootMaterialCategoriesTreeItem = new QTreeWidgetItem();
    rootMaterialCategoriesTreeItem->setText(0, "Все материалы");
    rootMaterialCategoriesTreeItem->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
    ui->materialsTreeWidget->insertTopLevelItem(0, rootMaterialCategoriesTreeItem);

    QList<QTreeWidgetItem *> items;
    while (query.next()) {
        items.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                                         query.value(0).toStringList()));
    }


    for (QTreeWidgetItem *item : items) {
        rootMaterialCategoriesTreeItem->addChild(item);
    }

    QTreeWidgetItem *rootWorkCategoriesTreeItem = new QTreeWidgetItem();
    rootWorkCategoriesTreeItem->setText(0, "Все работы");
    rootWorkCategoriesTreeItem->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties));
    ui->materialsTreeWidget->insertTopLevelItem(1, rootWorkCategoriesTreeItem);
}

// All the "set...ColumnWidth" functions will needs refactoring
// if the MaterialsWindow size is going change.
void MaterialsWindow::setMaterialsTableColumnWidth()
{
    ui->materialsTableWidget->setColumnWidth(0, 350);
    ui->materialsTableWidget->setColumnWidth(1, 100);
    ui->materialsTableWidget->setColumnWidth(2, 170);
    ui->materialsTableWidget->setColumnWidth(3, 130);
    ui->materialsTableWidget->setColumnWidth(4, 110);
    ui->materialsTableWidget->setColumnWidth(5, 110);
}

void MaterialsWindow::setMaterialsExtraOptionsTableColumnWidth()
{
    ui->materialsExtraOptionsTableWidget->setColumnWidth(0, 247);
    ui->materialsExtraOptionsTableWidget->setColumnWidth(1, 140);
    ui->materialsExtraOptionsTableWidget->setColumnWidth(2, 99);
}

void MaterialsWindow::setMaterialWorkAppropriatenessTableColumnWidth()
{
    ui->materialWorkAppropriatenessTableWidget->setColumnWidth(0, 243);
    ui->materialWorkAppropriatenessTableWidget->setColumnWidth(1, 243);
}
