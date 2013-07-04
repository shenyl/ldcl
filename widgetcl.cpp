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
    buttonUp = new QPushButton( tr(""), this );  //上升
    setButton( buttonUp, "background:url(:/images/up.png)");

    buttonDown = new QPushButton( tr("") ); //下降
    setButton( buttonDown, "background:url(:/images/down.png)");

    buttonHead = new QPushButton( tr("") );  //出车
    setButton( buttonHead, "background:url(:/images/right.png)");

    buttonBack = new QPushButton( tr("") );  //回车
    setButton( buttonBack, "background:url(:/images/left.png)");

    buttonStop = new QPushButton( tr("") );  //停止
    setButton( buttonStop, "background:url(:/images/stop.png)");

    buttonConfigQdj = new QPushButton( tr("设置起点距") );
    buttonConfigSs = new QPushButton( tr("设置水深") );
    buttonGetQdj = new QPushButton( tr("获取铅鱼当前位置") );
    buttonCs =  new QPushButton( tr("测速仪测速") );

    buttonClResult =  new QPushButton( tr("测量结果") );

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

    radioManual  = new QRadioButton( tr("手动测量") );
    radioManual->setChecked( true );

    radioFullAuto = new QRadioButton( tr("全自动测量") );
    radioHalfAuto = new QRadioButton( tr("半自动测量") );
    lblAutoState = new QLabel(tr("自动测量停止")) ;
    buttonClStart = new QPushButton( tr("开始") );
    buttonClPause = new QPushButton( tr("暂停") );
    buttonClStop = new QPushButton( tr("停止") );  
    buttonClUnFinish = new QPushButton( tr("查看未完成") );

    buttonClStop->setEnabled(false);
    buttonClPause->setEnabled(false);

    connect( buttonClStart, SIGNAL(clicked()), this, SLOT( slotClStart(  )));
    connect( buttonClStop, SIGNAL(clicked()), this, SLOT( slotClStop(  )));
    connect( buttonClPause, SIGNAL(clicked()), this, SLOT( slotClPause(  )));
    connect( buttonClUnFinish, SIGNAL(clicked()), this, SLOT( slotClUnfinish(  )));

    buttonHalfContinue = new QPushButton( tr("半自动继续") );
    buttonClearLog = new QPushButton( tr("清除日志") );
    buttonSaveLog = new QPushButton( tr("保存日志") );

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

    QGroupBox *groupBox = new QGroupBox(tr("自动测量"));
    groupBox->setLayout( LayoutAuto );
    groupBox->setMaximumWidth( 300  );

    label1 = new QLabel( tr("起点距:") );
    label2 = new QLabel( tr("水深:") );

    lblMsg = new QLabel( tr("反馈信息:") );

    pEdtQdj = new QLineEdit ;
    pEdtSs = new QLineEdit ;

    pWidCx = new QWidgetCxE( tr("cx") );

    connect( pWidCx, SIGNAL( sigQdjSS( float, float )), \
             this, SLOT( slotSetQdj( float , float ) ) );

    pWidDm = new QGraphicViewDm ;
    pWidDm->setWater( getSysconfig( 1 ).toFloat() );

    pPlainTextEdit = new QPlainTextEdit ;
    pPlainTextEdit->setMaximumBlockCount( 130 ); //最大行数

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

    threadserial.setComNo( getSysconfig( 0 ).toInt() ); //设串口
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
    threadserial.terminate();   //停止
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

//开始测量
void QWidgetCl::slotCmdCl( )
{
    threadserial.sendCmdCl( );
}

//设置起点距
void QWidgetCl::slotCmdConfigQdj( )
{
    threadserial.setConfigQdj( pEdtQdj->text() );
}

//设置水深
void QWidgetCl::slotCmdConfigSs( )
{
    threadserial.setConfigSs( pEdtSs->text() );
}

//设置按钮的样式
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

//获取当前起点距
void QWidgetCl::slotGetQdj( )
{
    threadserial.queryMove( );
}

//显示飞鱼结果
void QWidgetCl::slotFishPos( float *fPos , int iNums, bool bRes  )
{
    iNums;
    bRes ;
    QString str = QString( tr( "起点距%1 水深%2 预置起点距%3 预置水深%4" ) )
            .arg(*(fPos+3)).arg(*(fPos+4)).arg(*(fPos+5)).arg(*(fPos+6));

    slotGetMsg(str);

}

//显示流速结果
void QWidgetCl::slotClRes( float *fCl, int iNums, bool bRes )
{
    iNums;
    bRes ;
    QString str = QString( tr( "T %1 N%2 V%3" ) )
            .arg(*(fCl+2)).arg(*(fCl+3)).arg(*(fCl+4));

    slotGetMsg(str);

}

//获取参数设置指定项的值
QString QWidgetCl::getSysconfig( int iId )
{
    QSqlQuery query;
    QString strSql ;
    strSql = QString( "select content from sysconfig where id = %1" ).arg(iId) ;
    query.exec( strSql );

    QString strValue ;
    while ( query.next() ){         //断面中的水深就是高程
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

//查看测量结果
void QWidgetCl::slotClResult(  )
{
    QDlgResult dlg("result") ;
    dlg.exec();
}

//启动自动测量
void QWidgetCl::slotClStart(  )
{
    int iAutoMode ;
    if( radioManual->isChecked() ) iAutoMode = MODE_MANU ;
    if( radioFullAuto->isChecked() ) iAutoMode = MODE_AUTO ;
    if( radioHalfAuto->isChecked() ) iAutoMode = MODE_HALF ;

    if( iAutoMode != MODE_MANU )
        lblAutoState->setText(tr("自动测量开始"));
    else{
        lblAutoState->setText(tr("手动测量 测量开始"));
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
    lblAutoState->setText(tr("自动测量停止"));

    buttonClStart->setEnabled( true );
    buttonClStop->setEnabled( false );
    buttonClPause->setEnabled(false);
}

void QWidgetCl::slotClPause(  )
{
    if( buttonClPause->text() == tr("暂停") ){
        buttonClPause->setText( tr("继续") );
        threadserial.pauseAuto();
        lblAutoState->setText(tr("自动测量暂停"));
    }
    else{
        buttonClPause->setText( tr("暂停") );
        threadserial.continueAuto();
        lblAutoState->setText(tr("自动测量继续"));
    }
}

//查看未完成的任务
void QWidgetCl::slotClUnfinish(  )
{

}

//半自动化继续
void QWidgetCl::slotHalfContinue(  )
{
    threadserial.setHalfContinue();
    buttonHalfContinue->setEnabled( false );
}

//清日志
void QWidgetCl::slotClearLog(  )
{
    pPlainTextEdit->clear();
}

//保存日志
void QWidgetCl::slotSaveLog(  )
{
    QString fileName = QFileDialog::getSaveFileName( 0, tr("保存日志文件"),
                               QCoreApplication::applicationDirPath(  ),
                               tr("日志结果 (*.txt)"));

//    qDebug( ) << fileName ;

    QFile file( fileName );
    if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) )
        return ;

    QTextStream in(&file);
    in << pPlainTextEdit->toPlainText( );

}

//获取半自动的中途信号
void QWidgetCl::slotGetHalfContinue( )
{
    buttonHalfContinue->setEnabled( true );
    pPlainTextEdit->appendHtml( tr("<font color=\"#ff0000\">点击按钮继续半自动化的测速工作</font>") );
}

//自动测量结束槽
void QWidgetCl::slotGetAuto( )
{
    buttonClStart->setEnabled( true );
    buttonClPause->setEnabled( false );
    buttonClStop->setEnabled( false );

    pPlainTextEdit->appendHtml( tr("<font color=\"#ff0000\">自动测量结束</font>") );
}
