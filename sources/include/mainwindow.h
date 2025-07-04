#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "materialswindow.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showWithAccess(const int &access);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void openMaterialsWindow();

private:
    Ui::MainWindow *ui;
    MaterialsWindow *materialsWindow = nullptr;

    enum AccessLevel {
        FULL_ACCESS = 1,
        STRICT_ACCESS = 2
    };
};
#endif // MAINWINDOW_H
