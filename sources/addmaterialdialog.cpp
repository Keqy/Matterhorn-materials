#include "include/addmaterialdialog.h"
#include "ui_addmaterialdialog.h"
#include "include/material.h"

AddMaterialDialog::AddMaterialDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddMaterialDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->saveButton, &QPushButton::clicked, this, &AddMaterialDialog::saveAndExit);
    QObject::connect(ui->cancelButton, &QPushButton::clicked, this, &AddMaterialDialog::reject);
}

AddMaterialDialog::~AddMaterialDialog()
{
    delete ui;
}

void AddMaterialDialog::saveAndExit()
{
    material.name = ui->nameLine->text();
    material.measure = ui->measureLine->text();
    material.costPrice = ui->costPriceLine->text();
    material.minAmount = ui->minAmountLine->text();
    material.weight = ui->weightLine->text();
    material.wasteRate = ui->wasteRateLine->text();

    if (material.isFilled()) {
        QDialog::accept();
    } else {
        ui->errorLabel->setText("Необходимо заполнить все поля");
    }
}

Material AddMaterialDialog::getMaterial() const
{
    return material;
}
