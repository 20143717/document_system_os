#include "file_system.h"
#include "ui_file_system.h"

file_system::file_system(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::file_system)
{
    ui->setupUi(this);
}

file_system::~file_system()
{
    delete ui;
}
