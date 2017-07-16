#ifndef SHUXING_H
#define SHUXING_H

#include <QDialog>
#include <QString>

namespace Ui {
class shuxing;
}

class shuxing : public QDialog
{
    Q_OBJECT

public:
    explicit shuxing(QWidget *parent = 0);
    ~shuxing();
    QString a;
    QString b;
    QString c;
    QString d;
    QString e;
    void set(QString aa,QString bb,QString cc,QString dd,QString ee){
        this->a=aa;
        this->b=bb;
        this->c=cc;
        this->d=dd;
        this->e=ee;
    }

private:
    Ui::shuxing *ui;
protected:
    void showEvent(QShowEvent *);
};

#endif // SHUXING_H
