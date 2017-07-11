#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <QDialog>
#include "user.h"

namespace Ui {
class file_system;
}

class file_system : public QDialog
{
    Q_OBJECT

public:
    explicit file_system(QWidget *parent = 0);
    ~file_system();
    void format();
    void init();
    void run();
    void load();
    void writein();
private:
    Ui::file_system *ui;
protected:
    void showEvent(QShowEvent*);
};

#endif // FILE_SYSTEM_H
