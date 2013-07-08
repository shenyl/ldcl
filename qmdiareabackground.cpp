#include "qmdiareabackground.h"

#include <QPainter>

QMdiAreaBackGround::QMdiAreaBackGround( QWidget * parent )
    : QMdiArea( parent )
{
    bool b ;
    b = pix.load( ":/images/background.png" );
}

//×Ô»æÖÆÍ¼ÐÎ
void QMdiAreaBackGround::paintEvent( QPaintEvent * paintEvent )
{
    QMdiArea::paintEvent( paintEvent );

    QPainter painter(this->viewport());

    painter.drawPixmap( (width() - pix.width())/2, (height()-pix.height())/2, pix );
}
