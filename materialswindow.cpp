#include "materialswindow.h"
#include "ui_materialswindow.h"

MaterialsWindow::MaterialsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MaterialsWindow)
{
    ui->setupUi(this);
}

MaterialsWindow::~MaterialsWindow()
{
    delete ui;
}
