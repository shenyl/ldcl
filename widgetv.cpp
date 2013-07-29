#include <QVBoxLayout>
#include "widgetv.h"

QWidgetV::QWidgetV(QWidget *parent) : QWidget(parent)
{
    pGraphicViewV = new QGraphicViewV ;
    QHBoxLayout * mainLayout = new QHBoxLayout(this) ;
    mainLayout->addWidget( pGraphicViewV );
}
