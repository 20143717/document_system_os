#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <QDialog>

namespace Ui {
class file_system;
}

class file_system : public QDialog
{
    Q_OBJECT

public:
    explicit file_system(QWidget *parent = 0);
    ~file_system();

private:
    Ui::file_system *ui;
};

#endif // FILE_SYSTEM_H
