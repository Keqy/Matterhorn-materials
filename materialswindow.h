#ifndef MATERIALSWINDOW_H
#define MATERIALSWINDOW_H

#include "databasemanager.h"

#include <QSqlDatabase>
#include <QWidget>

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
    Ui::MaterialsWindow *ui;
    QSqlDatabase db;
    DatabaseManager dbManager;
    void updateMaterialsTree();
};

#endif // MATERIALSWINDOW_H
