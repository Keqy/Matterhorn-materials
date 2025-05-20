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
    QObject::connect(ui->materialsTreeWidget, &QTreeWidget::itemClicked, this, &MaterialsWindow::parseSelectedMaterialData);

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

void MaterialsWindow::parseSelectedMaterialData()
{
    QSqlDatabase db = QSqlDatabase::database("materials_connection");
    QSqlQuery query(db);
    QString materialName = ui->materialsTreeWidget->selectedItems()[0]->text(0);
    // Рефакторить на selectMaterialByName.
    CRUD::selectMaterialsByName(query, materialName);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
        return;
    }
    while (query.next()) {
        for (int i = 0; i < ui->materialsTableWidget->columnCount(); ++i){
            ui->materialsTableWidget->setRowCount(i+1);
            ui->materialsTableWidget->setItem(0, i, new QTableWidgetItem(query.value(i).toString())); //TODO: сделать для нескольких сделок;
        }
    }
}

void MaterialsWindow::parseMaterialCategoriesInRootTreeItem(QTreeWidgetItem *rootCategoriesTreeItem)
{
    QSqlDatabase db = QSqlDatabase::database("materials_connection");
    QSqlQuery query(db);
    CRUD::selectMaterialCategories(query);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
        return;
    }
    QList<QTreeWidgetItem *> materialCategories;
    while (query.next()) {
        materialCategories.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                                                      query.value(0).toStringList()));
    }
    for (QTreeWidgetItem *category : materialCategories) {
        category->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
        rootCategoriesTreeItem->addChild(category);
    }
}

void MaterialsWindow::updateMaterialsTree()
{
    ui->materialsTreeWidget->clear();

    QTreeWidgetItem *rootMaterialCategoriesTreeItem = new QTreeWidgetItem();
    rootMaterialCategoriesTreeItem->setText(0, "Все материалы");
    rootMaterialCategoriesTreeItem->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
    ui->materialsTreeWidget->insertTopLevelItem(0, rootMaterialCategoriesTreeItem);

    parseMaterialCategoriesInRootTreeItem(rootMaterialCategoriesTreeItem);

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
