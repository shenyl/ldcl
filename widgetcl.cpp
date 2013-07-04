#include "widgetcl.h"
#include "graphicviewdm.h"
#include "dlgresult.h"

#include "clscx.h"

#include <QVBoxLayout>
#include <QPalette>
#include <QGridLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QFileDialog>

QWidgetCl::QWidgetCl(QWidget *parent) :
    QWidget(parent)
{
    buttonUp = new QPushButton( tr(""), this );  //����
    setButton( buttonUp, "background:url(:/images/up.png)");

    buttonDown = new QPushButton( tr("") ); //�½�
    setButton( buttonDown, "background:url(:/images/down.png)");

    buttonHead = new QPushButton( tr("") );  //����
    setButton( buttonHead, "background:url(:/images/right.png)");

    buttonBack = new QPushButton( tr("") );  //�س�
    setButton( buttonBack, "background:url(:/images/left.png)");

    buttonStop = new QPushButton( tr("") );  //ֹͣ
    setButton( buttonStop, "background:url(:/images/stop.png)");

    buttonConfigQdj = new QPushButton( tr("��������") );
    buttonConfigSs = new QPushButton( tr("����ˮ��") );
    buttonGetQdj = new QPushButton( tr("��ȡǦ�㵱ǰλ��") );
    buttonCs =  new QPushButton( tr("�����ǲ���") );

    buttonClResult =  new QPushButton( tr("�������") );

    connect( buttonUp, SIGNAL(clicked()), this, SLOT( slotCmdUp( ) ));
    connect( buttonDown, SIGNAL(clicked()), this, SLOT( slotCmdDown( ) ));
    connect( buttonHead, SIGNAL(clicked()), this, SLOT( slotCmdHead( ) ));
    connect( buttonBack, SIGNAL(clicked()), this, SLOT( slotCmdBack( ) ));
    connect( buttonStop, SIGNAL(clicked()), this, SLOT( slotCmdStop( ) ));
    connect( buttonConfigQdj, SIGNAL(clicked()), this, SLOT( slotCmdConfigQdj( ) ));
    connect( buttonConfigSs, SIGNAL(clicked()), this, SLOT( slotCmdConfigSs( ) ));
    connect( buttonGetQdj, SIGNAL(clicked()), this, SLOT( slotGetQdj( ) ));

    connect( buttonCs, SIGNAL(clicked()), this, SLOT( slotCmdCl( ) ));
    connect( buttonClResult, SIGNAL(clicked()), this, SLOT( slotClResult( )));

    connect( &threadserial, SIGNAL( sigFishPos( float* , int, bool) ), \
             this, SLOT( slotFishPos( float *, int, bool) ));
    connect( &threadserial, SIGNAL( sigClRes( float * , int , bool ) ), \
             this, SLOT( slotClRes( float *, int, bool  ) ));
    connect( &threadserial, SIGNAL( sigSendMsg( QString ) ), \
             this, SLOT( slotGetMsg( QString ) ));
    connect( &threadserial, SIGNAL( sigHalf(  ) ), \
             this, SLOT( slotGetHalfContinue( ) ) );
    connect( &threadserial, SIGNAL( sigAuto() ), \
             this, SLOT( slotGetAuto( ) ) );

    radioManual  = new QRadioButton( tr("�ֶ�����") );
    radioManual->setChecked( true );

    radioFullAuto = new QRadioButton( tr("ȫ�Զ�����") );
    radioHalfAuto = new QRadioButton( tr("���Զ�����") );
    lblAutoState = new QLabel(tr("�Զ�����ֹͣ")) ;
    buttonClStart = new QPushButton( tr("��ʼ") );
    buttonClPause = new QPushButton( tr("��ͣ") );
    buttonClStop = new QPushButton( tr("ֹͣ") );  
    buttonClUnFinish = new QPushButton( tr("�鿴δ���") );

    buttonClStop->setEnabled(false);
    buttonClPause->setEnabled(false);

    connect( buttonClStart, SIGNAL(clicked()), this, SLOT( slotClStart(  )));
    connect( buttonClStop, SIGNAL(clicked()), this, SLOT( slotClStop(  )));
    connect( buttonClPause, SIGNAL(clicked()), this, SLOT( slotClPause(  )));
    connect( buttonClUnFinish, SIGNAL(clicked()), this, SLOT( slotClUnfinish(  )));

    buttonHalfContinue = new QPushButton( tr("���Զ�����") );
    buttonClearLog = new QPushButton( tr("�����־") );
    buttonSaveLog = new QPushButton( tr("������־") );

    buttonHalfContinue->setEnabled( false );

    connect( buttonHalfContinue, SIGNAL(clicked()), this, SLOT( slotHalfContinue(  )));
    connect( buttonClearLog, SIGNAL(clicked()), this, SLOT( slotClearLog( )));
    connect( buttonSaveLog, SIGNAL(clicked()), this, SLOT( slotSaveLog( )));

    QGridLayout * LayoutAuto = new QGridLayout ;
    LayoutAuto->addWidget(radioManual, 0, 0 );
    LayoutAuto->addWidget(radioFullAuto, 1, 0 );
    LayoutAuto->addWidget(radioHalfAuto, 1, 1);
    LayoutAuto->addWidget(lblAutoState, 3, 0,1,2);
    LayoutAuto->addWidget(buttonClStart, 4, 0);
//    LayoutAuto->addWidget(buttonClPause, 4, 1);
    LayoutAuto->addWidget(buttonClStop, 4, 1);
//    LayoutAuto->addWidget(buttonClUnFinish, 4, 1);

    QGroupBox *groupBox = new QGroupBox(tr("�Զ�����"));
    groupBox->setLayout( LayoutAuto );
    groupBox->setMaximumWidth( 300  );

    label1 = new QLabel( tr("����:") );
    label2 = new QLabel( tr("ˮ��:") );

    lblMsg = new QLabel( tr("������Ϣ:") );

    pEdtQdj = new QLineEdit ;
    pEdtSs = new QLineEdit ;

    pWidCx = new QWidgetCxE( tr("cx") );

    connect( pWidCx, SIGNAL( sigQdjSS( float, float )), \
             this, SLOT( slotSetQdj( float , float ) ) );

    pWidDm = new QGraphicViewDm ;
    pWidDm->setWater( getSysconfig( 1 ).toFloat() );

    pPlainTextEdit = new QPlainTextEdit ;
    pPlainTextEdit->setMaximumBlockCount( 130 ); //�������

    QHBoxLayout * LayoutGetQdj = new QHBoxLayout ;
    LayoutGetQdj->addStretch( );
    LayoutGetQdj->addWidget( buttonGetQdj );
    LayoutGetQdj->addWidget( buttonClResult );

    QHBoxLayout * Layout1 = new QHBoxLayout ;
    Layout1->addWidget( label1 );
    Layout1->addWidget( pEdtQdj );
    Layout1->addWidget( buttonConfigQdj );
    Layout1->addWidget( label2 );
    Layout1->addWidget( pEdtSs );
    Layout1->addWidget( buttonConfigSs );

    QHBoxLayout * Layout2 = new QHBoxLayout ;
    Layout2->addWidget( buttonGetQdj  );
    Layout2->addWidget( buttonCs );
    Layout2->addWidget( buttonClResult );

    QGridLayout * LayoutCtl = new QGridLayout ;
    LayoutCtl->addWidget(buttonUp, 0, 1 );
    LayoutCtl->addWidget(buttonHead, 1, 0);
    LayoutCtl->addWidget(buttonStop, 1, 1);
    LayoutCtl->addWidget(buttonBack, 1, 2);
    LayoutCtl->addWidget(buttonDown, 2, 1);

    QHBoxLayout * Layout3 = new QHBoxLayout ;
    Layout3->addLayout( LayoutCtl );
    Layout3->addWidget( groupBox );

    QHBoxLayout * Layout4 = new QHBoxLayout ;
    Layout4->addWidget( lblMsg );
    Layout4->addStretch();
    Layout4->addWidget( buttonHalfContinue );
    Layout4->addWidget( buttonClearLog );
    Layout4->addWidget( buttonSaveLog );

    QVBoxLayout * LayoutRight = new QVBoxLayout;
//    LayoutRight->addStretch( );
    LayoutRight->addWidget( pWidDm );
    LayoutRight->addLayout( Layout3 );
//    LayoutRight->addLayout( LayoutGetQdj );
    LayoutRight->addLayout( Layout1 );
    LayoutRight->addLayout( Layout2 );
    LayoutRight->addLayout( Layout4 );
    LayoutRight->addWidget( pPlainTextEdit );

    QHBoxLayout * mainLayout = new QHBoxLayout(this) ;
    mainLayout->addWidget( pWidCx );
    mainLayout->addLayout( LayoutRight );

    threadserial.setComNo( getSysconfig( 0 ).toInt() ); //�贮��
    threadserial.init();
    threadserial.start( );

//    char a[] = "aK0.2500 C0.0011 T001.5 N0000 V0.000";
//    char b[5];
//    float f1, f2;
//    sscanf(a, "%2c%f %c%f", b, &f1, b, &f2  );
//    qDebug( ) << f1 << f2 ;

}

void QWidgetCl::closeEvent ( QCloseEvent * event )
{
    threadserial.terminate();   //ֹͣ
    QWidget::closeEvent( event );
//    event->accept( );
}

void QWidgetCl::slotCmdUp( )
{
    threadserial.sendCmdMove( CMD_UP );
}

void QWidgetCl::slotCmdDown( )
{
    threadserial.sendCmdMove( CMD_DOWN );
}

void QWidgetCl::slotCmdHead( )
{
    threadserial.sendCmdMove( CMD_HEAD );
}

void QWidgetCl::slotCmdBack( )
{
    threadserial.sendCmdMove( CMD_BACK );
}

void QWidgetCl::slotCmdStop( )
{
    threadserial.sendCmdMove( CMD_STOP );
    threadserial.setStateNotStart( );
}

//��ʼ����
void QWidgetCl::slotCmdCl( )
{
    threadserial.sendCmdCl( );
}

//��������
void QWidgetCl::slotCmdConfigQdj( )
{
    threadserial.setConfigQdj( pEdtQdj->text() );
}

//����ˮ��
void QWidgetCl::slotCmdConfigSs( )
{
    threadserial.setConfigSs( pEdtSs->text() );
}

//���ð�ť����ʽ
void QWidgetCl::setButton( QPushButton * pButton, QString strStyle )
{
    pButton->setStyleSheet( strStyle );
//    const int iPushButtonWide = 128 ;
    const int iPushButtonWide = 48 ;
    pButton->setMinimumSize( iPushButtonWide, iPushButtonWide );
    pButton->setMaximumSize( iPushButtonWide, iPushButtonWide );
}

//
void QWidgetCl::slotSetQdj( float fQdj, float fSs )
{
    pEdtQdj->setText( QString("%1").arg(fQdj) );
    pEdtSs->setText( QString("%1").arg(fSs) );
}

//��ȡ��ǰ����
void QWidgetCl::slotGetQdj( )
{
    threadserial.queryMove( );
}

//��ʾ������
void QWidgetCl::slotFishPos( float *fPos , int iNums, bool bRes  )
{
    iNums;
    bRes ;
    QString str = QString( tr( "����%1 ˮ��%2 Ԥ������%3 Ԥ��ˮ��%4" ) )
            .arg(*(fPos+3)).arg(*(fPos+4)).arg(*(fPos+5)).arg(*(fPos+6));

    slotGetMsg(str);

}

//��ʾ���ٽ��
void QWidgetCl::slotClRes( float *fCl, int iNums, bool bRes )
{
    iNums;
    bRes ;
    QString str = QString( tr( "T %1 N%2 V%3" ) )
            .arg(*(fCl+2)).arg(*(fCl+3)).arg(*(fCl+4));

    slotGetMsg(str);

}

//��ȡ��������ָ�����ֵ
QString QWidgetCl::getSysconfig( int iId )
{
    QSqlQuery query;
    QString strSql ;
    strSql = QString( "select content from sysconfig where id = %1" ).arg(iId) ;
    query.exec( strSql );

    QString strValue ;
    while ( query.next() ){         //�����е�ˮ����Ǹ߳�
        strValue = query.value(0).toString();
    }
    return strValue ;
}

//
void QWidgetCl::slotGetMsg( QString strMsg )
{
    QDateTime dt = QDateTime::currentDateTime() ;
    QTime t = dt.time() ;

    QString strTime ;
    strTime.sprintf( "%02d:%02d:%02d   ", t.hour(),t.minute(),t.second() );

    pPlainTextEdit->appendPlainText( strTime + strMsg );
}

//�鿴�������
void QWidgetCl::slotClResult(  )
{
    QDlgResult dlg("result") ;
    dlg.exec();
}

//�����Զ�����
void QWidgetCl::slotClStart(  )
{
    int iAutoMode ;
    if( radioManual->isChecked() ) iAutoMode = MODE_MANU ;
    if( radioFullAuto->isChecked() ) iAutoMode = MODE_AUTO ;
    if( radioHalfAuto->isChecked() ) iAutoMode = MODE_HALF ;

    if( iAutoMode != MODE_MANU )
        lblAutoState->setText(tr("�Զ�������ʼ"));
    else{
        lblAutoState->setText(tr("�ֶ����� ������ʼ"));
        return ;
    }

    clsCx  cx ;
    QSqlTableModel *  tableModel ;
    tableModel =  pWidCx->getTableModel( );

    threadserial.clearCx( );

    QSqlRecord record;

    int iCxid;
    float fQdj, fSs ;
    int iClff, iCzff ;

    for(int i=0; i<tableModel->rowCount(); i++){
        record = tableModel->record(i);
        iCxid = record.value("cxid").toInt( );
        fQdj = record.value("qdj").toFloat();
        fSs = record.value("ss").toFloat();
        iClff = record.value("clff").toInt( );
        iCzff = record.value("czff").toInt( );
//        qDebug() << iCxid << fQdj << fSs << iClff << iCzff ;

        if( iCzff != 0 ){
            clsCx cx;
            cx.setMem( iCxid, fQdj, fSs, iClff, iCzff );
            threadserial.appendCx( cx );
        }
    }
//    threadserial.printfCx( );
    threadserial.startAutoMode( iAutoMode );

    buttonClStart->setEnabled(false);
    buttonClStop->setEnabled(true);
    buttonClPause->setEnabled(true);
}

void QWidgetCl::slotClStop(  )
{
    threadserial.stopAuto();
    lblAutoState->setText(tr("�Զ�����ֹͣ"));

    buttonClStart->setEnabled( true );
    buttonClStop->setEnabled( false );
    buttonClPause->setEnabled(false);
}

void QWidgetCl::slotClPause(  )
{
    if( buttonClPause->text() == tr("��ͣ") ){
        buttonClPause->setText( tr("����") );
        threadserial.pauseAuto();
        lblAutoState->setText(tr("�Զ�������ͣ"));
    }
    else{
        buttonClPause->setText( tr("��ͣ") );
        threadserial.continueAuto();
        lblAutoState->setText(tr("�Զ���������"));
    }
}

//�鿴δ��ɵ�����
void QWidgetCl::slotClUnfinish(  )
{

}

//���Զ�������
void QWidgetCl::slotHalfContinue(  )
{
    threadserial.setHalfContinue();
    buttonHalfContinue->setEnabled( false );
}

//����־
void QWidgetCl::slotClearLog(  )
{
    pPlainTextEdit->clear();
}

//������־
void QWidgetCl::slotSaveLog(  )
{
    QString fileName = QFileDialog::getSaveFileName( 0, tr("������־�ļ�"),
                               QCoreApplication::applicationDirPath(  ),
                               tr("��־��� (*.txt)"));

//    qDebug( ) << fileName ;

    QFile file( fileName );
    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return ;

    QTextStream in(&file);
    in << pPlainTextEdit->toPlainText( );

}

//��ȡ���Զ�����;�ź�
void QWidgetCl::slotGetHalfContinue( )
{
    buttonHalfContinue->setEnabled( true );
    pPlainTextEdit->appendHtml( tr("<font color=\"#ff0000\">�����ť�������Զ����Ĳ��ٹ���</font>") );
}

//�Զ�����������
void QWidgetCl::slotGetAuto( )
{
    buttonClStart->setEnabled( true );
    buttonClPause->setEnabled( false );
    buttonClStop->setEnabled( false );

    pPlainTextEdit->appendHtml( tr("<font color=\"#ff0000\">�Զ���������</font>") );
}
