#include "include/addmaterialdialog.h"
#include "ui_addmaterialdialog.h"

AddMaterialDialog::AddMaterialDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddMaterialDialog)
{
    ui->setupUi(this);
}

AddMaterialDialog::~AddMaterialDialog()
{
    delete ui;
}
