#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->materialsButton, &QPushButton::clicked, this, &MainWindow::materialsButtonClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::materialsButtonClicked()
{

}
