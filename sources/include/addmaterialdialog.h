#ifndef ADDMATERIALDIALOG_H
#define ADDMATERIALDIALOG_H

#include "material.h"

#include <QDialog>

namespace Ui {
class AddMaterialDialog;
}

class AddMaterialDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMaterialDialog(QWidget *parent = nullptr);
    ~AddMaterialDialog();

    Material getMaterial() const;

private:
    Ui::AddMaterialDialog *ui;

    Material material;

    void saveAndExit();
};

#endif // ADDMATERIALDIALOG_H
