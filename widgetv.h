#ifndef WIDGETV_H
#define WIDGETV_H

#include <QWidget>
#include "graphicviewv.h"

//�ٶ��ߵ���ʾ

class QWidgetV : public QWidget
{
    Q_OBJECT

public:
    explicit QWidgetV(QWidget *parent = 0);

    QGraphicViewV * pGraphicViewV ;

signals:

public slots:

};

#endif // WIDGETV_H
