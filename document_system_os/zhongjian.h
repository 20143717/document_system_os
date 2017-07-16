#ifndef ZHONGJIAN_H
#define ZHONGJIAN_H

#include <QString>

class zhongjian
{
public:
    QString thing;
    bool save;
public:
    zhongjian(){
        this->thing="";
        this->save=0;
    }
    void set(QString x,bool y){
        this->thing=x;
        this->save=y;
    }
};

#endif // ZHONGJIAN_H
