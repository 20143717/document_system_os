#ifndef LOGIN_H
#define LOGIN_H


#include <QMainWindow>
#include <QString>
#include "ui_mainwindow.h"
#include "define.h"

/*struct mainFileDirectory {
    int iNode;
    mainFileDirectoryItem item[DIRECTORYITEMNUM];
    int sz;

    mainFileDirectory() {
        iNode = -1;
        sz = DIRECTORYITEMNUM;
        for (int i = 0; i<DIRECTORYITEMNUM; i++) {
            item[i].iNode = -1;
            strcpy(item[i].name,"/" );
            strcpy(item[i].psw,"/" );
        }
    }
    int size() {
        return sz;
    }
    void set(int id, mainFileDirectoryItem t) {
        item[id] = t;
    }
    void init() {
        iNode = -1;
        sz = DIRECTORYITEMNUM;
        for (int i = 0; i<DIRECTORYITEMNUM; i++) {
            item[i].iNode = -1;
            strcpy(item[i].name,"/" );
            strcpy(item[i].psw,"/" );
        }
    }
    mainFileDirectoryItem get(int i) {
        return item[i];
    }
    int add(char *name, int id) { //name为name的i结点id
        for (int i = 0; i<DIRECTORYITEMNUM; i++) {
            if (item[i].iNode == -1) {
                item[i].iNode = id;
                strcpy(item[i].name, name);
                //cout << "分配的SFD ID是" << i << endl;
                return i; // 返回的是SFD的位置
            }
        }
        return -1;
    }
    void drop(int i) {
        item[i].iNode = -1;
    }
};

class Login{
public:
    mainFileDirectory MFD;

};

namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~lexical();

private slots:
    void on_pushButton_clicked();


private:
    Ui::Login *ui;
};
*/
#endif // LOGIN_H
