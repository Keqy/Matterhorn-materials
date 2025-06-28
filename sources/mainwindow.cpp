#include "include/mainwindow.h"
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

void MainWindow::showWithAccess(const int &access)
{
    switch (access) {
    case AccessLevel::FULL_ACCESS:
        ui->calculatorButton->setEnabled(true);
        ui->materialsButton->setEnabled(true);
        break;
    case AccessLevel::STRICT_ACCESS:
        ui->calculatorButton->setEnabled(true);
        ui->materialsButton->setEnabled(false);
        break;
    default:
        break;
    }

    this->show();
}

void MainWindow::openMaterialsWindow()
{
    if (!materialsWindow) {
        materialsWindow = new MaterialsWindow();
        materialsWindow->setAttribute(Qt::WA_DeleteOnClose);
        materialsWindow->show();

        // Deletes the pointer when the window is destroed.
        QObject::connect(materialsWindow, &QWidget::destroyed, this, [this]() { materialsWindow = nullptr; });
    } else {
        materialsWindow->showNormal();
        materialsWindow->raise();
        materialsWindow->activateWindow();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // TODO: Close other windows, too.
    if (materialsWindow) {
        materialsWindow->close();
    }
    // Continues with the usual closeEvent.
    QMainWindow::closeEvent(event);
}
