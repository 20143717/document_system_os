#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <admin.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    admin Admin;
private:
    Ui::MainWindow *ui;
private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
