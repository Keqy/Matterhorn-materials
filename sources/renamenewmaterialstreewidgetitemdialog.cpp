#include "include/renamenewmaterialstreewidgetitemdialog.h"
#include "ui_renamenewmaterialstreewidgetitemdialog.h"

RenameNewMaterialsTreeWidgetItemDialog::RenameNewMaterialsTreeWidgetItemDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RenameNewMaterialsTreeWidgetItemDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->saveAndExitButton, &QPushButton::clicked, this, &RenameNewMaterialsTreeWidgetItemDialog::saveAndExit);
}

RenameNewMaterialsTreeWidgetItemDialog::~RenameNewMaterialsTreeWidgetItemDialog()
{
    delete ui;
}

QString RenameNewMaterialsTreeWidgetItemDialog::getNewName() const
{
    return ui->lineEdit->text();
}

void RenameNewMaterialsTreeWidgetItemDialog::saveAndExit()
{
    QDialog::accept();
}
