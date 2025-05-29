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
    QObject::connect(ui->materialsTableWidgetAccessButton, &QPushButton::clicked, this, &MaterialsWindow::changeMaterialsTableWidgetAccess);
    QObject::connect(ui->addButton, &QPushButton::clicked, this, &MaterialsWindow::addMaterialsTableWidgetRow);
    QObject::connect(ui->removeButton, &QPushButton::clicked, this, &MaterialsWindow::removeMaterialsTableWidgetRow);

    QSqlDatabase db;
    DatabaseManager dbManager;
    dbManager.setupDatabaseConnection(db, "materials_connection", "config/db.config");
    if (!dbManager.lastError().isEmpty()) {
        QMessageBox::critical(this, "Ошибка соединения с БД", dbManager.lastError());
    }

    setMaterialsTableColumnWidth();
    setMaterialsExtraOptionsTableColumnWidth();
    setMaterialWorkAppropriatenessTableColumnWidth();
    updateMaterialsTreeWidget();
}

MaterialsWindow::~MaterialsWindow()
{
    delete ui;
    db.removeDatabase("materials_connection");
}

void MaterialsWindow::updateMaterialsTreeWidget()
{
    ui->materialsTreeWidget->clear();

    // Materials.
    QTreeWidgetItem *rootMaterialCategoriesTreeWidgetItem = new QTreeWidgetItem();
    rootMaterialCategoriesTreeWidgetItem->setText(0, "Все материалы");
    rootMaterialCategoriesTreeWidgetItem->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
    ui->materialsTreeWidget->insertTopLevelItem(0, rootMaterialCategoriesTreeWidgetItem);

    QList<QTreeWidgetItem *> materialCategories = getMaterialCategories();
    parseMaterialCategoriesInRootTreeWidgetItem(materialCategories, rootMaterialCategoriesTreeWidgetItem);
    parseMaterialTypesInCategoryTreeWidgetItems(materialCategories);

    // Works.
    QTreeWidgetItem *rootWorkCategoriesTreeWidgetItem = new QTreeWidgetItem();
    rootWorkCategoriesTreeWidgetItem->setText(0, "Все работы");
    rootWorkCategoriesTreeWidgetItem->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties));
    ui->materialsTreeWidget->insertTopLevelItem(1, rootWorkCategoriesTreeWidgetItem);
}

QList<QTreeWidgetItem *> MaterialsWindow::getMaterialCategories()
{
    QSqlDatabase db = QSqlDatabase::database("materials_connection");
    QSqlQuery query(db);
    QList<QTreeWidgetItem *> materialCategories;

    CRUD::selectMaterialCategories(query);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
    }
    while (query.next()) {
        materialCategories.append(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr),
                                                      query.value(0).toStringList()));
    }
    return materialCategories;
}

void MaterialsWindow::parseMaterialCategoriesInRootTreeWidgetItem(QList<QTreeWidgetItem *> categories, QTreeWidgetItem *rootTreeItem)
{
    for (QTreeWidgetItem *category : categories) {
        category->setIcon(0, QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen));
        rootTreeItem->addChild(category);
    }
}

void MaterialsWindow::parseMaterialTypesInCategoryTreeWidgetItems(QList<QTreeWidgetItem *> categories)
{
    QSqlDatabase db = QSqlDatabase::database("materials_connection");
    QSqlQuery query(db);

    for (QTreeWidgetItem *category : categories) {
        CRUD::selectMaterialTypesByCategory(query, category->text(0));
        if (query.lastError().isValid()) {
            QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
        }
        while (query.next()) {
            category->addChild(new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), query.value(0).toStringList()));
        }
    }
}

// --- Signals
void MaterialsWindow::parseSelectedMaterialData()
{
    setMaterialsTableWidgetLock(true);
    ui->materialsTableWidget->clearContents();
    ui->materialsTableWidget->setRowCount(0);

    QSqlDatabase db = QSqlDatabase::database("materials_connection");
    QSqlQuery query(db);
    QString typeName = ui->materialsTreeWidget->selectedItems()[0]->text(0);

    CRUD::selectMaterialsByType(query, typeName);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
        return;
    }
    query.first(); // REFACTOR THIS...
    if (query.isValid())
    {
        ui->materialsTableWidget->setEnabled(true);
        ui->materialsTableWidget->setRowCount(query.size());
        int row = 0;
        do { // AND THIS.
            for (int i = 0; i < ui->materialsTableWidget->columnCount(); ++i){
                ui->materialsTableWidget->setItem(row, i, new QTableWidgetItem(query.value(i).toString()));
            }
            ++row;
        } while (query.next());
    }
}

inline void MaterialsWindow::changeMaterialsTableWidgetAccess()
{
    setMaterialsTableWidgetLock(!isMaterialsTableWidgetLocked);
}

inline void MaterialsWindow::addMaterialsTableWidgetRow()
{
    if (!isMaterialsTableWidgetLocked) {
        int rowCount = ui->materialsTableWidget->rowCount();
        ui->materialsTableWidget->setRowCount(rowCount + 1);
    }
}

inline void MaterialsWindow::removeMaterialsTableWidgetRow()
{
    if (!isMaterialsTableWidgetLocked) {
        int currentRow = ui->materialsTableWidget->currentRow();
        ui->materialsTableWidget->removeRow(currentRow);
    }
}

// --- UI
// All the "set...ColumnWidth" functions will needs refactoring
// if the MaterialsWindow size is going change.
inline void MaterialsWindow::setMaterialsTableColumnWidth()
{
    ui->materialsTableWidget->setColumnWidth(0, 350);
    ui->materialsTableWidget->setColumnWidth(1, 100);
    ui->materialsTableWidget->setColumnWidth(2, 170);
    ui->materialsTableWidget->setColumnWidth(3, 130);
    ui->materialsTableWidget->setColumnWidth(4, 110);
    ui->materialsTableWidget->setColumnWidth(5, 100);
}

inline void MaterialsWindow::setMaterialsExtraOptionsTableColumnWidth()
{
    ui->materialsExtraOptionsTableWidget->setColumnWidth(0, 247);
    ui->materialsExtraOptionsTableWidget->setColumnWidth(1, 140);
    ui->materialsExtraOptionsTableWidget->setColumnWidth(2, 99);
}

inline void MaterialsWindow::setMaterialWorkAppropriatenessTableColumnWidth()
{
    ui->materialWorkAppropriatenessTableWidget->setColumnWidth(0, 243);
    ui->materialWorkAppropriatenessTableWidget->setColumnWidth(1, 243);
}

void MaterialsWindow::setMaterialsTableWidgetLock(const bool &lock)
{
    if (lock) {
        ui->materialsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->materialsTableWidgetAccessButton->setText("Разблокировать");
        isMaterialsTableWidgetLocked = true;
    } else {
        // Unlock.
        if (isMaterialTypeSelected()) {
            ui->materialsTableWidget->setEditTriggers(QAbstractItemView::CurrentChanged);
            ui->materialsTableWidgetAccessButton->setText("Заблокировать");
            isMaterialsTableWidgetLocked = false;
        }
    }
}

bool MaterialsWindow::isMaterialTypeSelected() const
{
    QTreeWidgetItem *item = ui->materialsTreeWidget->currentItem();
    return item && item->parent() && item->parent()->parent();
}

// void MaterialsWindow::closeEvent(QCloseEvent *event)
// {
//     MaterialsWindow::closeEvent(event);
// }
