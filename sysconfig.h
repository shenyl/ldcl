#ifndef SYSCONFIG_H
#define SYSCONFIG_H

#include <QVector>
#include <QObject>

#define     POS_COM     1   //
#define     POS_BAUD    2   //

class QSysConfigItem
{
public:
    int id;
    QString strContent ;
    QString strMemo ;
};


class QSysConfig
{
public:
    QSysConfig();

    QVector<QSysConfigItem> vectorSysConfig;

    void init(  );

    int  getValue( int id );

};

#endif // SYSCONFIG_H
