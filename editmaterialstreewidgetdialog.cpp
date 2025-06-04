#include "editmaterialstreewidgetdialog.h"
#include "ui_editmaterialstreewidgetdialog.h"
#include "renamenewmaterialstreewidgetitemdialog.h"

EditMaterialsTreeWidgetDialog::EditMaterialsTreeWidgetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditMaterialsTreeWidgetDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->addCategoryButton, &QPushButton::clicked, this, &EditMaterialsTreeWidgetDialog::addCategory);
    QObject::connect(ui->addTypeButton, &QPushButton::clicked, this, &EditMaterialsTreeWidgetDialog::addType);
    QObject::connect(ui->renameItemButton, &QPushButton::clicked, this, &EditMaterialsTreeWidgetDialog::renameMaterialsTreeWidgetItem);
}

EditMaterialsTreeWidgetDialog::~EditMaterialsTreeWidgetDialog()
{
    delete ui;
}

void EditMaterialsTreeWidgetDialog::addCategory()
{
    if (isMaterialRootSelected()) {
        QModelIndex parentIndex = ui->materialsTreeView->currentIndex();
        ui->materialsTreeView->expand(parentIndex);
        ui->materialsTreeView->model()->insertRow(0, parentIndex);

        QModelIndex newIndex = ui->materialsTreeView->model()->index(0, 0, parentIndex);
        ui->materialsTreeView->model()->setData(newIndex, "Новая категория");
        if (parentIndex.data().toString() == "Все материалы") {
            ui->materialsTreeView->model()->setData(newIndex, QIcon::fromTheme(QIcon::ThemeIcon::FolderOpen), Qt::DecorationRole);
        } else if (parentIndex.data().toString() == "Все работы") {
            ui->materialsTreeView->model()->setData(newIndex, QIcon::fromTheme(QIcon::ThemeIcon::DocumentProperties), Qt::DecorationRole);
        }
        ui->materialsTreeView->setCurrentIndex(newIndex);
        ui->materialsTreeView->scrollTo(newIndex);
    }
}

void EditMaterialsTreeWidgetDialog::addType()
{
    if (isMaterialCategorySelected()) {
        QModelIndex parentIndex = ui->materialsTreeView->currentIndex();
        ui->materialsTreeView->expand(parentIndex);
        ui->materialsTreeView->model()->insertRow(0, parentIndex);

        QModelIndex newIndex = ui->materialsTreeView->model()->index(0, 0, parentIndex);
        ui->materialsTreeView->model()->setData(newIndex, "Новая подкатегория");
        ui->materialsTreeView->setCurrentIndex(newIndex);
        ui->materialsTreeView->scrollTo(newIndex);
    }
}

bool EditMaterialsTreeWidgetDialog::isMaterialRootSelected()
{
    QModelIndex index = ui->materialsTreeView->currentIndex();
    return index.isValid() && !(index.parent().isValid());
}

bool EditMaterialsTreeWidgetDialog::isMaterialCategorySelected()
{
    QModelIndex index = ui->materialsTreeView->currentIndex();
    return index.isValid() && index.parent().isValid() && !(index.parent().parent().isValid());
}

void EditMaterialsTreeWidgetDialog::renameMaterialsTreeWidgetItem()
{
    RenameNewMaterialsTreeWidgetItemDialog renameDialog;
    if (renameDialog.exec()) {
        QModelIndex index = ui->materialsTreeView->currentIndex();
        QString newName;
        newName = renameDialog.getNewName();
        ui->materialsTreeView->model()->setData(index, newName);
    }

}

void EditMaterialsTreeWidgetDialog::setMaterialsTreeView(QAbstractItemModel *model)
{
    ui->materialsTreeView->setModel(model);
}
