#pragma once

#ifndef ADDEXTRAMATERIALOPTIONDIALOG_H
#define ADDEXTRAMATERIALOPTIONDIALOG_H

#include "extramaterialoption.h"

#include <QDialog>

namespace Ui {
class AddExtraMaterialOptionDialog;
}

class AddExtraMaterialOptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddExtraMaterialOptionDialog(QWidget *parent = nullptr);
    ~AddExtraMaterialOptionDialog();

    const ExtraMaterialOption getOption() const;

private:
    Ui::AddExtraMaterialOptionDialog *ui;
    void saveAndExit();
    ExtraMaterialOption option;
};

#endif // ADDEXTRAMATERIALOPTIONDIALOG_H
