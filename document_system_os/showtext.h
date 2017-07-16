#ifndef SHOWTEXT_H
#define SHOWTEXT_H

#include <QDialog>

namespace Ui {
class showtext;
}

class showtext : public QDialog
{
    Q_OBJECT

public:
    explicit showtext(QWidget *parent = 0);
    ~showtext();
    QString thing;
    QString getstring();
    void set(QString x){
        this->thing=x;
    }
    QPushButton *a;

private:
    Ui::showtext *ui;
protected:
    void showEvent(QShowEvent *);

private slots:
    void on_pushButton_1_clicked();
};

#endif // SHOWTEXT_H
