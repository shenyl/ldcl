#ifndef WIDGETCL_H
#define WIDGETCL_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QRadioButton>

#include "./serial/qserialthread.h"
#include "widgetcxe.h"
class QGraphicViewDm;

class QWidgetCl : public QWidget
{
    Q_OBJECT
public:
    explicit QWidgetCl(QWidget *parent = 0);

    QSerialThread threadserial ;

    QPushButton * buttonUp ;
    QPushButton * buttonDown ;
    QPushButton * buttonHead ;
    QPushButton * buttonBack ;
    QPushButton * buttonStop ;

    QPushButton * buttonCs ;
    QPushButton * buttonConfigQdj ;
    QPushButton * buttonConfigSs ;
    QPushButton * buttonGetQdj ;
    QPushButton * buttonClResult;

    QRadioButton * radioManual ;
    QRadioButton * radioFullAuto;
    QRadioButton * radioHalfAuto;
    QLabel * lblAutoState ;
    QPushButton * buttonClStart ;
    QPushButton * buttonClPause ;
    QPushButton * buttonClStop  ;
    QPushButton * buttonClUnFinish ;

    QLabel * label1, *label2;
    QLabel * lblGetQdj;
    QLineEdit * pEdtQdj, * pEdtSs ;
    QLabel * lblMsg ;

    QWidgetCxE * pWidCx ;
    QGraphicViewDm * pWidDm ;

    QPlainTextEdit * pPlainTextEdit ;

private slots:
    void slotCmdUp( );
    void slotCmdDown( );
    void slotCmdHead( );
    void slotCmdBack( );
    void slotCmdStop( );

    void slotCmdCl( );
    void slotCmdConfigQdj( );
    void slotCmdConfigSs( );
    void slotSetQdj( float fQdj, float fSs );
    void slotGetQdj( );

    void slotClStart(  );
    void slotClStop(  );
    void slotClPause(  );
    void slotClUnfinish(  );

    void slotFishPos( float *, int iNums, bool bRes  );
    void slotClRes( float *, int iNums, bool bRes  );
    void slotGetMsg( QString strMsg );
    void slotClResult(  );

private:
    void setButton( QPushButton * pButton, QString strStyle );
    QString getSysconfig( int iId );

protected:
    void closeEvent ( QCloseEvent * event );
signals:

public slots:

};

#endif // WIDGETCL_H
