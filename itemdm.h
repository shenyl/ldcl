#ifndef ITEMDM_H
#define ITEMDM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>

class ItemDm : public QGraphicsItem
{
public:
    ItemDm( );
    void init( QRectF rect );

    QRectF boundingRect() const;

    void setWater( float fGcWater );  //设水面高程
    QPointF getFishPos( float fQdj, float fBelowWater );

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

};

#endif // ITEMDM_H
