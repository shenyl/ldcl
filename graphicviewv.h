#ifndef GRAPHICVIEWV_H
#define GRAPHICVIEWV_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>

#include "itemv.h"

//速度线的QGraphicView
class QGraphicViewV : public QGraphicsView
{
    Q_OBJECT
public:
    explicit QGraphicViewV( QWidget *parent = 0 );

    ItemV *  getItem( ){ return pItemV ; }

protected:
    void resizeEvent ( QResizeEvent * event );

private:
    QGraphicsScene * scene ;

    ItemV *  pItemV ;

signals:

public slots:

};

#endif // GRAPHICVIEWDM_H
