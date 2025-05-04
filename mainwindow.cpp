#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    if (!materialsWindow) {
        materialsWindow = new MaterialsWindow();
        materialsWindow->setAttribute(Qt::WA_DeleteOnClose);
        materialsWindow->show();

        // Убрать указатель при закрытии окна.
        QObject::connect(materialsWindow, &QWidget::destroyed, this, [this]() { materialsWindow = nullptr; });
    } else {
        materialsWindow->raise();
        materialsWindow->activateWindow();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Закрыть остальные окна.
    if (materialsWindow) {
        materialsWindow->close();
    }
    // Продолжить стандартное закрытие окна.
    QMainWindow::closeEvent(event);
}
