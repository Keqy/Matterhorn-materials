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

void AddMaterialDialog::setMaterialName(const QString &name)
{
    ui->nameLine->setText(name);
}

void AddMaterialDialog::saveAndExit()
{
    material.name = ui->nameLine->text();
    material.measure = ui->measureBox->currentText();
    material.costPrice = ui->priceBox->text();
    material.minAmount = ui->minAmountBox->text();
    material.weight = ui->weightBox->text();
    material.wasteRate = ui->wasteRateBox->text();

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
