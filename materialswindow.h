#ifndef MATERIALSWINDOW_H
#define MATERIALSWINDOW_H

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

private:
    Ui::MaterialsWindow *ui;
    QSqlDatabase db;
};

#endif // MATERIALSWINDOW_H
