#include "showtext.h"
#include "ui_showtext.h"
#include <QCloseEvent>

showtext::showtext(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showtext)
{
    ui->setupUi(this);
    a=new QPushButton("保存",this);
    a->setGeometry(40,270,75,23);
}

showtext::~showtext()
{
    delete ui;
}
void showtext::showEvent(QShowEvent *){
    this->resize(400,300);
    ui->textEdit->setText(thing);
}
void showtext::on_pushButton_1_clicked(){
    this->reject();
}
QString showtext::getstring(){
    thing=ui->textEdit->toPlainText();
    return thing;
}
