#include "graphicviewv.h"

#include <QGraphicsRectItem>
#include <QGraphicsPolygonItem>
#include <QResizeEvent>

#include <QDebug>
#include <math.h>
#include <QPolygonF>

QGraphicViewV::QGraphicViewV(QWidget *parent) :
    QGraphicsView(parent)
{
    int iWidth , iHeight ;

    iWidth = 600 ;
    iHeight = 230 ;

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(-iWidth/2,-iHeight/2,iWidth,iHeight);   //(0,0)在中心位置
    setScene(scene);
    setCacheMode(CacheBackground);

    //加一个矩形
    QRectF r = QRectF(-iWidth/2, -iHeight/2, iWidth, iHeight );
//    qDebug( ) <<  r.topLeft() << r.bottomRight() ;

    QRectF r1  = r.adjusted( 20, 20, -20, -20 );
//    qDebug( ) <<  r1.topLeft() << r1.bottomRight() ;

    QGraphicsRectItem * pRect = scene->addRect( r1 );  //创建用的是对象座标
    pRect->setPos( 0, 0 );    //给的是场景座标

    pItemV = new ItemV ;

    // 4/5 的宽度  2/3的高度
    float  fAdjust1, fAdjust2 ;

    fAdjust1 = 1.0 / 10.0 * iWidth  ;
    fAdjust2 = 1.0 / 6.0 * iHeight  ;

//    qDebug( ) << fAdjust1 << fAdjust2 << r.adjusted( fAdjust1 , fAdjust2, -1*fAdjust1, -1*fAdjust2 ) ;
    pItemV->init( r.adjusted( fAdjust1 , fAdjust2, -1*fAdjust1, -1*fAdjust2 ) );

    scene->addItem( pItemV );
    pItemV->setPos( 0, 0 );
}

void QGraphicViewV::resizeEvent ( QResizeEvent * event )
{
//    qDebug() << "size" <<  event->size(  );
}
