#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <QDialog>
#include "user.h"
#include <QMouseEvent>
#include "showtext.h"
#include "shuxing.h"
#include <QLibrary>

namespace Ui {
class file_system;
}

class file_system : public QDialog
{
    Q_OBJECT

public:
    explicit file_system(QWidget *parent = 0);
    ~file_system();
    QString choose_name;
    QString choose_type;
    QString choose_path;
    QString thing;
    showtext ShowText;
    shuxing SX;
    void addUser();

private slots:
    void format();
    void init();
    void run();
    void load();
    void vim();
    void writein();
    int iNodeMalloc();
    void sudoFormat();
    int getINodeOfCurrentPath();
    void cd_back();
    void cd();
    void pwd();
    void ls();
    void ll();
    int sfdMalloc(int getINode);
    void rmf();
    void mkdir();
    void rename(QString dir1,QString dir2);
    void touch();
    int createFile(QString fname);
    void renamef(QString dir1,QString dir2);
    void cat();
    void rm();
    void on_pushButton_clicked();
    void on_pushButton_1_clicked();
private:
    Ui::file_system *ui;
protected:
    void showEvent(QShowEvent*);
    void contextMenuEvent(QContextMenuEvent * event );
};

#endif // FILE_SYSTEM_H
