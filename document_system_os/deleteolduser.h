#ifndef DELETEOLDUSER_H
#define DELETEOLDUSER_H

#include <QDialog>

namespace Ui {
class DeleteOldUser;
}

class DeleteOldUser : public QDialog
{
    Q_OBJECT

public:
    explicit DeleteOldUser(QWidget *parent = 0);
    ~DeleteOldUser();

private:
    Ui::DeleteOldUser *ui;
private slots:
    void on_certain_clicked();
    void on_clear_clicked();

};

#endif // DELETEOLDUSER_H
