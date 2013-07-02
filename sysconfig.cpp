#include "sysconfig.h"

#include <QtSql>

//沈宇亮   2014-05-20
//参数配置使用类
QSysConfig::QSysConfig()
{
}

const int iMaxVectorSize = 100 ;

void QSysConfig::init( )
{
    vectorSysConfig.resize( iMaxVectorSize );

    QSqlQuery query;

    QString strSql ;

    strSql = QString( "select id, content, memo from sysconfig" );
    query.exec( strSql );

    QSysConfigItem sys;

    while ( query.next() ){
        sys.id = query.value(0).toInt();
        sys.strContent = query.value(1).toString();
        sys.strMemo = query.value(2).toString();
        vectorSysConfig[sys.id] = sys ;
    }
}

int  QSysConfig::getValue( int id )
{
    if( id >= iMaxVectorSize || id < 0  )  return -1 ;
    return vectorSysConfig[id].strContent.toInt( );
}
