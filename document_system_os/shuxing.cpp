#include "shuxing.h"
#include "ui_shuxing.h"

shuxing::shuxing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shuxing)
{
    ui->setupUi(this);
}

shuxing::~shuxing()
{
    delete ui;
}
void shuxing::showEvent(QShowEvent *){
    this->resize(386,289);
    ui->label->setText(a);
    ui->label_2->setText(b);
    ui->label_3->setText(c);
    ui->label_4->setText(d);
    ui->label_5->setText(e);
}
