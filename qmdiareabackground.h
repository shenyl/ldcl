#ifndef QMDIAREABACKGROUND_H
#define QMDIAREABACKGROUND_H

#include <QMdiArea>
#include <QPixmap>

//������MDI����������ʾһ��ͼ��
class QMdiAreaBackGround : public QMdiArea
{
public:
    QMdiAreaBackGround( QWidget * parent = 0 );

    QPixmap  pix ;

protected:
    void paintEvent( QPaintEvent * paintEvent );
};

#endif // QMDIAREABACKGROUND_H
