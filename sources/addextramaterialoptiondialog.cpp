#include "include/addextramaterialoptiondialog.h"
#include "ui_addextramaterialoptiondialog.h"

AddExtraMaterialOptionDialog::AddExtraMaterialOptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddExtraMaterialOptionDialog)
{
    ui->setupUi(this);
    QObject::connect(ui->saveButton, &QPushButton::clicked, this, &AddExtraMaterialOptionDialog::saveAndExit);
}

AddExtraMaterialOptionDialog::~AddExtraMaterialOptionDialog()
{
    delete ui;
}

void AddExtraMaterialOptionDialog::saveAndExit()
{
    option.name = ui->nameLine->text();
    option.value = ui->valueBox->text();
    option.measure = ui->measureBox->currentText();
    if (option.isFilled()) {
        AddExtraMaterialOptionDialog::accept();
    } else {
        ui->errorLabel->setText("Необходимо заполнить поля");
        return;
    }
}

const ExtraMaterialOption AddExtraMaterialOptionDialog::getOption() const
{
    return option;
}
