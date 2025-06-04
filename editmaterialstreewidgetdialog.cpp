#include "editmaterialstreewidgetdialog.h"
#include "ui_editmaterialstreewidgetdialog.h"

EditMaterialsTreeWidgetDialog::EditMaterialsTreeWidgetDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditMaterialsTreeWidgetDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->addCategoryButton, &QPushButton::clicked, this, &EditMaterialsTreeWidgetDialog::addCategory);
}

EditMaterialsTreeWidgetDialog::~EditMaterialsTreeWidgetDialog()
{
    delete ui;
}

void EditMaterialsTreeWidgetDialog::addCategory()
{
    ui->treeView->model()->insertRow();
}

void EditMaterialsTreeWidgetDialog::setMaterialsTreeView(QAbstractItemModel *model)
{
    ui->treeView->setModel(model);
}
