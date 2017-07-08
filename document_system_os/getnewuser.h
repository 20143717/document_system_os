#ifndef GETNEWUSER_H
#define GETNEWUSER_H

#include <QDialog>
#include "user.h"

namespace Ui {
class GetNewUser;
}

class GetNewUser : public QDialog
{
    Q_OBJECT

public:
    explicit GetNewUser(QWidget *parent = 0);
    ~GetNewUser();
private:
    Ui::GetNewUser *ui;

private slots:
    void on_certain_clicked();
    void on_clear_clicked();
};

#endif // GETNEWUSER_H
