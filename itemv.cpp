#include "itemv.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>

ItemV::ItemV( )
{

}

//读数据库完成断面点集
//QRectF rect 是断面的绘制区域
void ItemV::init( QRectF rect )
{
    rect1 = rect ;

    QSqlQuery query;
    QString strSql ;

    //求最大最小高程 最大最小起点距,从断面表中获取
    strSql = QString( "select max(ss), min(ss), max(qdj), min(qdj) from dm" );
    query.exec( strSql );

    while ( query.next() ){         //断面中的水深就是高程
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

QRectF ItemV::boundingRect() const
{
    return poly.boundingRect( );
}

void ItemV::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawRect( rect1 );
    painter->drawPolyline( poly );
    painter->drawLine( fQdjMinPos, fWaterPos, fQdjMaxPos, fWaterPos );

    //绘制速度线
    QPen penLine;
    penLine.setWidth(2);
    penLine.setColor( QColor(255,0,0) );
    penLine.setStyle( Qt::SolidLine );
    painter->setPen( penLine );

    int i;
    QPointF point1, point2 ;
    float fQdj, fV ;
    for(i=0; i<listQdj.size(); i++){
        fQdj = listQdj.at(i);
        fV = listV.at(i);
        point1.setX( fQdjMinPos + (fQdj - fQdjMin) / (fQdjMax - fQdjMin) *rect1.width() );
        point1.setY( fGcMaxPos );

        point2.setX( point1.x() );
        point2.setY( fGcMaxPos +  fV / fMaxV * rect1.height() );

        painter->drawLine( point1, point2 );
    }
}

//设置水面高程，同时计算出水面的图形Y轴位置
void ItemV::setWater( float fGcWater )
{
    this->fGcWater = fGcWater ;
    fWaterPos = fGcMinPos - ( fGcWater - fGcMin) / (fGcMax - fGcMin) *rect1.height() ;
    update( rect1 );
}

//从控制柜过来的数据
//float fQdj  起点距
//float fBelowWater   水面下的深度
QPointF ItemV::getFishPos( float fQdj, float fBelowWater )
{
    QPointF point ;

    point.setX( fQdjMinPos + (fQdj+fQdjOffset - fQdjMin) / (fQdjMax - fQdjMin) *rect1.width() );

    float fGc ;  //当前铅鱼位置的高程
    fGc = fGcWater - fBelowWater ;

    point.setY( fGcMinPos - ( fGc - fGcMin) / (fGcMax - fGcMin) *rect1.height() );

    return point ;
}

//清除线列表
void ItemV::clearLine( )
{
    listQdj.clear( );
    listV.clear( );
}

//增加一条速度线
void ItemV::addVLine(  float fQdj, float fV )
{
    listQdj.append( fQdj );
    listV.append( fV );
}

//进行速度线的绘制
void ItemV::drawLine( )
{
    fMaxV = 0;

    int i;
    for(i=0; i<listV.size(); i++){
        if( listV.at(i) > fMaxV ) fMaxV = listV.at(i);
    }
    update( rect1 );
}
