#include "include/materialswindow.h"
#include "include/databasemanager.h"
#include "ui_materialswindow.h"
#include "include/editmaterialstreewidgetdialog.h"
#include "include/addmaterialdialog.h"

#include <QMessageBox>
#include <QSqlError>

MaterialsWindow::MaterialsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MaterialsWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->materialsTreeWidget, &QTreeWidget::itemClicked, this, &MaterialsWindow::parseSelectedTypeMaterials);
    QObject::connect(ui->addButton, &QPushButton::clicked, this, &MaterialsWindow::execAddMaterialDialog);
    QObject::connect(ui->removeButton, &QPushButton::clicked, this, &MaterialsWindow::removeMaterial);
    QObject::connect(ui->editButton, &QPushButton::clicked, this, &MaterialsWindow::execEditMaterialsTreeWidgetDialog);

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
void MaterialsWindow::parseSelectedTypeMaterials()
{
    ui->materialsTableWidget->clearContents();
    ui->materialsTableWidget->setRowCount(0);

    QSqlDatabase db = QSqlDatabase::database("materials_connection");
    QSqlQuery query(db);

    QString itemName = ui->materialsTreeWidget->selectedItems()[0]->text(0);
    //TODO: Try to rethink this implementation.
    if (isMaterialTypeSelected()) {
        CRUD::selectMaterialsByType(query, itemName);
    } else if (isMaterialCategorySelected()) {
        CRUD::selectMaterialsByCategory(query, itemName);
    } else if (isMaterialRootSelected()) {
        CRUD::selectMaterialsByName(query, "");
    } else {
        return;
    }
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
        return;
    }

    query.first(); // REFACTOR THIS...
    if (query.isValid())
    {
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

void MaterialsWindow::removeMaterial()
{
    int currentRow = ui->materialsTableWidget->currentRow();
    // If the row is not selected.
    if (currentRow < 0) {
        QMessageBox::information(this,
                                 "Не выбран материал",
                                 "Выберите материал для удаления");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("materials_connection");
    QSqlQuery query(db);

    int materialId = ui->materialsTableWidget->item(currentRow, 0)->text().toInt();

    QMessageBox msg(this);
    msg.setIcon(QMessageBox::Question);
    msg.setWindowTitle("Удаление материала");
    msg.setText("Вы уверены, что хотите удалить выделенный материал?");
    msg.addButton(QMessageBox::Yes)->setText("Да");
    msg.addButton(QMessageBox::No)->setText("Нет");
    msg.setWindowIcon(QIcon(":/images/logo/images/icon.ico"));
    if (msg.exec() == QMessageBox::Yes) {
        CRUD::deleteMaterial(query, materialId);
        if (query.lastError().isValid()) {
            QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
            return;
        }
        ui->materialsTableWidget->removeRow(currentRow);
    }
}

void MaterialsWindow::execEditMaterialsTreeWidgetDialog()
{
    EditMaterialsTreeWidgetDialog editMaterialsTreeWidgetDialog;
    editMaterialsTreeWidgetDialog.setMaterialsTreeView(ui->materialsTreeWidget->model());
    if (!editMaterialsTreeWidgetDialog.exec()) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("materials_connection");
    QSqlQuery query(db);
    if (!db.transaction()) {
        QMessageBox::critical(this, "Не удалось начать транзакцию в базу данных", db.lastError().text());
        return;
    }
    // TODO: Refactor change hierarchy. Maybe move it to the function.
    QList<TreeChange> changes = editMaterialsTreeWidgetDialog.getChanges();
    for (auto it = changes.begin(); it != changes.end(); ) {
        TreeChange change = *it;
        if (change.itemType == ItemType::Category) {
            if (change.changeType == ChangeType::Delete) {
                CRUD::deleteCategory(query, change);
                if (query.lastError().isValid()) {
                    QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
                    db.rollback();
                    break;
                }
            } else if (change.changeType == ChangeType::Insert) {
                CRUD::insertCategory(query, change);
                if (query.lastError().isValid()) {
                    QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
                    db.rollback();
                    break;
                }
            }
            it = changes.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = changes.begin(); it != changes.end(); ) {
        TreeChange change = *it;
        if (change.itemType == ItemType::Type) {
            if (change.changeType == ChangeType::Delete) {
                CRUD::deleteType(query, change);
                if (query.lastError().isValid()) {
                    QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
                    db.rollback();
                    break;
                }
            } else if (change.changeType == ChangeType::Insert) {
                CRUD::insertType(query, change);
                if (query.lastError().isValid()) {
                    QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
                    db.rollback();
                    break;
                }
            }
            it = changes.erase(it);
        } else {
            ++it;
        }
    }
    if (!db.commit()) {
        QMessageBox::critical(this, "Не удалось завершить транзакцию в базу данных", db.lastError().text());
    }
    updateMaterialsTreeWidget();
}

void MaterialsWindow::execAddMaterialDialog()
{
    if (!isMaterialTypeSelected()) {
        QMessageBox::information(this,
                                 "Невозможно добавить материал",
                                 "Выберите подкатегорию, в которую будет добавляться материал");
        return;
    }

    AddMaterialDialog addMaterialDialog;
    QString typeName = ui->materialsTreeWidget->currentItem()->text(0);
    addMaterialDialog.setMaterialName(typeName);

    if (!addMaterialDialog.exec()) {
        return;
    }

    QSqlDatabase db = QSqlDatabase::database("materials_connection");
    QSqlQuery query(db);
    Material newMaterial = addMaterialDialog.getMaterial();

    CRUD::insertMaterial(query, typeName, newMaterial);
    if (query.lastError().isValid()) {
        QMessageBox::critical(this, "Ошибка запроса к базе данных", query.lastError().text());
        return;
    }
    parseSelectedTypeMaterials();
}

// --- UI
// All the "set...ColumnWidth" functions will needs refactoring
// if the MaterialsWindow size is going change.
inline void MaterialsWindow::setMaterialsTableColumnWidth()
{
    ui->materialsTableWidget->hideColumn(0);
    ui->materialsTableWidget->setColumnWidth(1, 350);
    ui->materialsTableWidget->setColumnWidth(2, 100);
    ui->materialsTableWidget->setColumnWidth(3, 170);
    ui->materialsTableWidget->setColumnWidth(4, 130);
    ui->materialsTableWidget->setColumnWidth(5, 110);
    ui->materialsTableWidget->setColumnWidth(6, 100);
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

bool MaterialsWindow::isMaterialTypeSelected() const
{
    QTreeWidgetItem *item = ui->materialsTreeWidget->currentItem();
    return item && item->parent() && item->parent()->parent();
}

bool MaterialsWindow::isMaterialCategorySelected() const
{
    QTreeWidgetItem *item = ui->materialsTreeWidget->currentItem();
    return item && item->parent() && !(item->parent()->parent());
}

bool MaterialsWindow::isMaterialRootSelected() const
{
    QTreeWidgetItem *item = ui->materialsTreeWidget->currentItem();
    return item && !(item->parent());
}

// void MaterialsWindow::closeEvent(QCloseEvent *event)
// {
//     MaterialsWindow::closeEvent(event);
// }
