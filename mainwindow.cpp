#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "materialswindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->materialsButton, &QPushButton::clicked, this, &MainWindow::openMaterialsWindow);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openMaterialsWindow()
{
    // TODO: Сделать закрытие materialsWindow вместе с mainWindow.
    // if (!materialsWindow) {
    //     materialsWindow = new MaterialsWindow(this);
    //     materialsWindow->setWindowFlag(Qt::Window, true);
    //     materialsWindow->setAttribute(Qt::WA_DeleteOnClose);
    //     materialsWindow->show();

    //     QObject::connect(materialsWindow, &QWidget::destroyed, this, [this]() { materialsWindow = nullptr; });
    // }
    // else {
    //     materialsWindow->raise();
    //     materialsWindow->activateWindow();
    // }
}
