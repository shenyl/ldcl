#ifndef GRAPHICVIEWDM_H
#define GRAPHICVIEWDM_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

#include "itemdm.h"

class QGraphicViewDm : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QGraphicViewDm( QWidget *parent = 0 );

    void setFishPos( float fQdj, float fBelowWater, bool bIsMoving );
public slots:
    void setWater( QString strWaterGc );

protected:
    void resizeEvent ( QResizeEvent * event );
private:
    QGraphicsScene * scene ;

    QGraphicsEllipseItem * pFish ;
    ItemDm *  pItemDm ;

signals:

public slots:

};

#endif // GRAPHICVIEWDM_H
