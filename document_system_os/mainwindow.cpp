#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    QString name=ui->lineEdit->text();
    QString password=ui->lineEdit_2->text();
    if(name=="admin"&&password=="123456"){
        MessageBox:
    }
}
