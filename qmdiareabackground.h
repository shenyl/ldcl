#ifndef QMDIAREABACKGROUND_H
#define QMDIAREABACKGROUND_H

#include <QMdiArea>
#include <QPixmap>

//用于在MDI窗口中央显示一幅图形
class QMdiAreaBackGround : public QMdiArea
{
public:
    QMdiAreaBackGround( QWidget * parent = 0 );

    QPixmap  pix ;

protected:
    void paintEvent( QPaintEvent * paintEvent );
};

#endif // QMDIAREABACKGROUND_H
