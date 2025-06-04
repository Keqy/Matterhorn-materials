#ifndef RENAMENEWMATERIALSTREEWIDGETITEMDIALOG_H
#define RENAMENEWMATERIALSTREEWIDGETITEMDIALOG_H

#include <QDialog>

namespace Ui {
class RenameNewMaterialsTreeWidgetItemDialog;
}

class RenameNewMaterialsTreeWidgetItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameNewMaterialsTreeWidgetItemDialog(QWidget *parent = nullptr);
    ~RenameNewMaterialsTreeWidgetItemDialog();

    QString getNewName() const;

private:
    Ui::RenameNewMaterialsTreeWidgetItemDialog *ui;

    void saveAndExit();
};

#endif // RENAMENEWMATERIALSTREEWIDGETITEMDIALOG_H
