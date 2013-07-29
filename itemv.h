#ifndef ITEMV_H
#define ITEMV_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>

//速度线的自定义显示元件
class ItemV : public QGraphicsItem
{
public:
    ItemV( );
    void init( QRectF rect );

    QRectF boundingRect() const;

    void setWater( float fGcWater );  //设水面高程
    QPointF getFishPos( float fQdj, float fBelowWater );

    void clearLine( );
    void addVLine(  float fQdj, float fV );
    void drawLine( );

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
     QPolygonF  poly ;
     QRectF rect1 ;

     float fGcMin, fGcMax ;         //
     float fGcMinPos, fGcMaxPos ;   //

     float fQdjMin, fQdjMax ;       //
     float fQdjMinPos, fQdjMaxPos ; //

     float fGcWater ;               //水平面的高程
     float fWaterPos ;              //水平面的位置

     float fQdjOffset ;
     float fMaxV ;

     QList <float> listQdj ;        //起点距列表
     QList <float> listV ;          //速度列表

};

#endif // ItemV_H
