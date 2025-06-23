#include "include/addmaterialdialog.h"
#include "ui_addmaterialdialog.h"

AddMaterialDialog::AddMaterialDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddMaterialDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->saveButton, &QPushButton::clicked, this, &AddMaterialDialog::saveAndExit);
}

AddMaterialDialog::~AddMaterialDialog()
{
    delete ui;
}

void AddMaterialDialog::saveAndExit()
{
    if (isAllLinesFilled()) {
        QDialog::accept();
    } else {
        ui->errorLabel->setText("Необходимо заполнить все поля");
    }
}

bool AddMaterialDialog::isAllLinesFilled()
{
    if (ui->nameLine->text().isEmpty() ||
        ui->measureLine->text().isEmpty() ||
        ui->costPriceLine->text().isEmpty() ||
        ui->minAmountLine->text().isEmpty() ||
        ui->weightLine->text().isEmpty() ||
        ui->wasteRateLine->text().isEmpty()) {
        return false;
    }
    return true;
}
