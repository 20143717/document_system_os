#ifndef ADMIN_H
#define ADMIN_H

#include <QDialog>
#include "user.h"
#include <QPushButton>
#include "getnewuser.h"
extern USER user;
namespace Ui {
class admin;

}

class admin : public QDialog
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = 0);
    ~admin();

private:
    Ui::admin *ui;
    GetNewUser getnewuser;
private slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

};

#endif // ADMIN_H
