#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <QDialog>
#include "user.h"
#include <QMouseEvent>

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
    int iNodeMalloc();
    void sudoFormat();
    int getINodeOfCurrentPath();
    void cd_back();
    void cd(QString dir);
    void pwd();
    void ls();
    void ll();
    int sfdMalloc(int getINode);
    void rmf(QString dir);
    void mkdir();
    void rename(QString dir1,QString dir2);
    void touch();
    int createFile(QString fname);
    void vim(QString file);
    void renamef(QString dir1,QString dir2);
    void cat(QString file);
    QString choose_name;
    QString choose_type;

private:
    Ui::file_system *ui;
protected:
    void showEvent(QShowEvent*);
    void contextMenuEvent(QContextMenuEvent * event );
};

#endif // FILE_SYSTEM_H
