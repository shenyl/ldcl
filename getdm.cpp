#include "getdm.h"
#include <QSqlQuery>
#include <QVariant>
#include <QPointF>
#include <QLineF>

QGetDm::QGetDm(QObject *parent) :
    QObject(parent)
{
}

//����ˮ��߳��������ȡ�õ��ˮ��
//������û�м���ƫ�Ƶľ���
float  QGetDm::getSs( float fWaterSurface, float fQdj )
{
    float fQdjAddOffset ;
    fQdjAddOffset = fQdj + getSysconfig( 5 ).toFloat( );
    int i, iCount ;
    iCount = listDm.count( );
    int iIndex = -1 ;
    for(i=0; i<iCount; i++){
        if( listDm.at(i).fQdj >= fQdjAddOffset ) {
            iIndex = i;
            break ;
        }
    }

    if( iIndex == -1 )  return -1;

    QLineF line1( listDm.at(iIndex-1).fQdj, listDm.at(iIndex-1).fSs,  \
                  listDm.at(iIndex).fQdj, listDm.at(iIndex).fSs ) ;

    QLineF line2( fQdjAddOffset, 0, fQdjAddOffset, 2000 );

    QPointF  point ;
    line2.intersect( line1, &point );

    return fWaterSurface - point.y( );
}

//����ˮ��߳��뷽���ȡ��ˮ��߳���ߵ��������ұߵ�����
//0 ���������   1 ���ұߵ�����
float QGetDm::getQdj(  float fWaterSurface, int iDir )
{
    if( iDir == 0 )   return getQdjLeft( fWaterSurface );
    if( iDir == 1 )   return  getQdjRight( fWaterSurface );
    return -1 ;
}

float QGetDm::getQdjLeft( float fWaterSurface )
{
    int i, iCount ;
    iCount = listDm.count( );
    int iIndex = -1 ;

    for(i=0; i<iCount; i++){
        if( listDm.at(i).fSs <= fWaterSurface ) {
            iIndex = i;
            break ;
        }
    }

    if( iIndex == -1 )  return -1;

    QLineF line1( listDm.at(iIndex-1).fQdj, listDm.at(iIndex-1).fSs,  \
                  listDm.at(iIndex).fQdj, listDm.at(iIndex).fSs ) ;

    QLineF line2( 0, fWaterSurface, 500, fWaterSurface );

    QPointF  point ;
    line2.intersect( line1, &point );
    return point.x( );

}

float QGetDm::getQdjRight( float fWaterSurface )
{
    int i, iCount ;
    iCount = listDm.count( );
    int iIndex = -1 ;


    for(i=iCount-1; i>=0 ; i--){
        if( listDm.at(i).fSs <= fWaterSurface ) {
            iIndex = i;
            break ;
        }
    }

    if( iIndex == -1 )  return -1;

    QLineF line1( listDm.at(iIndex+1).fQdj, listDm.at(iIndex+1).fSs,  \
                  listDm.at(iIndex).fQdj, listDm.at(iIndex).fSs ) ;

    QLineF line2( 0, fWaterSurface, 500, fWaterSurface );

    QPointF  point ;
    line2.intersect( line1, &point );
    return point.x( );
}

//��ȡ��������ָ�����ֵ
QString QGetDm::getSysconfig( int iId )
{
    QSqlQuery query;
    QString strSql ;
    strSql = QString( "select content from sysconfig where id = %1" ).arg(iId) ;
    query.exec( strSql );

    QString strValue ;
    while ( query.next() ){
        strValue = query.value(0).toString();
    }
    return strValue ;
}

//��ѯ������Ϣ����������
void  QGetDm::readDm( )
{
    QSqlQuery query;
    QString strSql ;
    strSql = QString( "select id, qdj, ss from dm order by id " );
    query.exec( strSql );

    clsDm  dm ;
    while ( query.next() ){
        dm.iId = query.value(0).toInt();
        dm.fQdj = query.value(1).toFloat();
        dm.fSs = query.value(2).toFloat( );
        listDm << dm ;
    }

}

int QGetDm::getQdjLeftIndex( float fWaterSurface )
{
    int i, iCount ;
    iCount = listDm.count( );
    int iIndex = -1 ;

    for(i=0; i<iCount; i++){
        if( listDm.at(i).fSs <= fWaterSurface ) {
            iIndex = i;
            break ;
        }
    }
    if( iIndex == -1 )  return -1;

    return listDm.at(iIndex - 1).iId ;
}

int QGetDm::getQdjRightIndex( float fWaterSurface )
{
    int i, iCount ;
    iCount = listDm.count( );
    int iIndex = -1 ;


    for(i=iCount-1; i>=0 ; i--){
        if( listDm.at(i).fSs <= fWaterSurface ) {
            iIndex = i;
            break ;
        }
    }

    if( iIndex == -1 )  return -1;

    return listDm.at(iIndex + 1).iId ;
}
