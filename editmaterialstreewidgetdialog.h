#ifndef EDITMATERIALSTREEWIDGETDIALOG_H
#define EDITMATERIALSTREEWIDGETDIALOG_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class EditMaterialsTreeWidgetDialog;
}

class EditMaterialsTreeWidgetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditMaterialsTreeWidgetDialog(QWidget *parent = nullptr);
    ~EditMaterialsTreeWidgetDialog();

    void setMaterialsTreeView(QAbstractItemModel *model);

private:
    Ui::EditMaterialsTreeWidgetDialog *ui;

    void addCategory();
};

#endif // EDITMATERIALSTREEWIDGETDIALOG_H
