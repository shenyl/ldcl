#ifndef ITEMV_H
#define ITEMV_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>

//�ٶ��ߵ��Զ�����ʾԪ��
class ItemV : public QGraphicsItem
{
public:
    ItemV( );
    void init( QRectF rect );

    QRectF boundingRect() const;

    void setWater( float fGcWater );  //��ˮ��߳�
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

     float fGcWater ;               //ˮƽ��ĸ߳�
     float fWaterPos ;              //ˮƽ���λ��

     float fQdjOffset ;
     float fMaxV ;

     QList <float> listQdj ;        //�����б�
     QList <float> listV ;          //�ٶ��б�

};

#endif // ItemV_H
