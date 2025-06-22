#ifndef EDITMATERIALSTREEWIDGETDIALOG_H
#define EDITMATERIALSTREEWIDGETDIALOG_H

#include "treechangetracker.h"

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

    const QList<TreeChange>& getChanges() const { return changeTracker->getChanges(); };

private:
    Ui::EditMaterialsTreeWidgetDialog *ui;
    void renameMaterialsTreeWidgetItem();
    void addCategory();
    void addType();
    void removeCategory();
    void removeType();

    bool isMaterialRootSelected();
    bool isMaterialCategorySelected();
    bool isMaterialTypeSelected();

    TreeChangeTracker *changeTracker;
};

#endif // EDITMATERIALSTREEWIDGETDIALOG_H
