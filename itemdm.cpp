#include "itemdm.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>

ItemDm::ItemDm( )
{

}

//�����ݿ���ɶ���㼯
//QRectF rect �Ƕ���Ļ�������
void ItemDm::init( QRectF rect )
{
    rect1 = rect ;

    QSqlQuery query;
    QString strSql ;

    //�������С�߳� �����С����,�Ӷ�����л�ȡ
    strSql = QString( "select max(ss), min(ss), max(qdj), min(qdj) from dm" );
    query.exec( strSql );

    while ( query.next() ){         //�����е�ˮ����Ǹ߳�
        fGcMax = query.value(0).toFloat( );
        fGcMin = query.value(1).toFloat( );
        fQdjMax = query.value(2).toFloat( );
        fQdjMin = query.value(3).toFloat( );
    }

    fGcMinPos  = rect1.bottom();
    fGcMaxPos  = rect1.top( );
    fQdjMinPos  = rect1.left() ;
    fQdjMaxPos  = rect1.right() ;

    fWaterPos = rect1.top( );

//    qDebug( ) << rect1 ;
//    qDebug( ) << "max min" << fGcMaxPos << fGcMinPos ;

    strSql = QString( "select id, qdj, ss from dm order by id" );
    query.exec( strSql );

    int id ;
    float fqdj, fss ;
    QPointF point ;

    while ( query.next() ){
        id = query.value(0).toInt( );
        fqdj = query.value(1).toFloat( );
        fss = query.value(2).toFloat( );

        point.setX( fQdjMinPos + (fqdj - fQdjMin) / (fQdjMax - fQdjMin) *rect1.width() );
        point.setY( fGcMinPos - ( fss - fGcMin) / (fGcMax - fGcMin) *rect1.height() );
        poly << point ;
//        qDebug( ) << "point " <<  point << fss << fGcMin  ;
    }

    strSql = QString( "select content from sysconfig where id = 5" ) ;
    query.exec( strSql );

    QString strValue ;
    while ( query.next() ){
        strValue = query.value(0).toString();
        fQdjOffset = strValue.toFloat( );
    }

}

QRectF ItemDm::boundingRect() const
{
    return poly.boundingRect( );
}

void ItemDm::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect( rect1 );
    painter->drawPolyline( poly );
    painter->drawLine( fQdjMinPos, fWaterPos, fQdjMaxPos, fWaterPos );
}

//����ˮ��̣߳�ͬʱ�����ˮ���ͼ��Y��λ��
void ItemDm::setWater( float fGcWater )
{
    this->fGcWater = fGcWater ;
    fWaterPos = fGcMinPos - ( fGcWater - fGcMin) / (fGcMax - fGcMin) *rect1.height() ;
    //update( boundingRect() );
    update( rect1 );

}

//�ӿ��ƹ����������
//float fQdj  ����
//float fBelowWater   ˮ���µ����
QPointF ItemDm::getFishPos( float fQdj, float fBelowWater )
{
    QPointF point ;

    point.setX( fQdjMinPos + (fQdj+fQdjOffset - fQdjMin) / (fQdjMax - fQdjMin) *rect1.width() );

    float fGc ;  //��ǰǦ��λ�õĸ߳�
    fGc = fGcWater - fBelowWater ;

    if( fBelowWater < 0 )
        point.setY( fWaterPos );
    else
        point.setY( fGcMinPos - ( fGc - fGcMin) / (fGcMax - fGcMin) *rect1.height() );

    return point ;
}
