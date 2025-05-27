#ifndef MATERIALSWINDOW_H
#define MATERIALSWINDOW_H

#include "databasemanager.h"

#include <QSqlDatabase>
#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
class MaterialsWindow;
}

class MaterialsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MaterialsWindow(QWidget *parent = nullptr);
    ~MaterialsWindow();

signals:
    void errorOccurred(const QString &title, const QString &error);

private:
    // UI.
    Ui::MaterialsWindow *ui;
    bool isMaterialsTableWidgetLocked = true;

    inline void setMaterialsTableColumnWidth();
    inline void setMaterialsExtraOptionsTableColumnWidth();
    inline void setMaterialWorkAppropriatenessTableColumnWidth();
    void updateMaterialsTreeWidget();
    void parseSelectedMaterialData();
    void parseMaterialCategoriesInRootTreeWidgetItem(QList<QTreeWidgetItem *> categories, QTreeWidgetItem *rootTreeItem);
    void parseMaterialTypesInCategoryTreeWidgetItems(QList<QTreeWidgetItem *> categories);

    // Database interactions.
    QSqlDatabase db;
    DatabaseManager dbManager;

    QList<QTreeWidgetItem *> getMaterialCategories();
    void addMaterialsTableWidgetRow();
    void changeMaterialsTableWidgetAccess();
    void removeMaterialsTableWidgetRow();
};

#endif // MATERIALSWINDOW_H
