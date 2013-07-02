#include "ldcl.h"
#include "widgetcl.h"
#include "widgetcxe.h"
#include "qmdiareabackground.h"

//#include "QWidgetManage.h"
//#include "qdlglogin.h"

#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QMdiArea>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QFile>
#include <QApplication>
#include <QToolButton>
#include <QToolBar>
#include <QMdiSubWindow>

QLdcl::QLdcl(QWidget* parent, Qt::WindowFlags flags)
	: QMainWindow(parent, flags)
{
    createMenusAndActions();

    m_mdiArea = new QMdiAreaBackGround;
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);

    setCentralWidget(m_mdiArea);//设为中央窗口部件

    connectDatabase();

    setWindowTitle(tr("缆道测速系统"));

    showMaximized( );
}

QLdcl::~QLdcl()
{

}

void QLdcl::createMenusAndActions()
{
    //功能菜单
    m_menuFunction = menuBar()->addMenu(tr("&F功能"));

    m_actConfigDm = new QAction(tr("断面设置"), this);
    m_actConfigDm->setToolTip(tr("断面设置"));
    m_actConfigDm->setShortcut( QKeySequence(tr("Ctrl+A")) );
    m_menuFunction->addAction(m_actConfigDm);

    connect(m_actConfigDm, SIGNAL(triggered()), this, SLOT( slotConfigDm( ) ));

    m_actSysconfig = new QAction(tr("系统设置"), this);
    m_actSysconfig->setToolTip(tr("系统设置"));
    m_actSysconfig->setShortcut( QKeySequence(tr("Ctrl+C")) );
    m_menuFunction->addAction(m_actSysconfig);

    connect(m_actSysconfig, SIGNAL(triggered()), this, SLOT( slotSysconfig( ) ));


    m_actCl = new QAction(tr("测量"), this);
    m_actCl->setToolTip(tr("测量)"));
    m_actCl->setShortcut( QKeySequence(tr("Ctrl+B")) );
    m_menuFunction->addAction(m_actCl);
    connect(m_actCl, SIGNAL(triggered()), this, SLOT(slotCl()));

//    m_actInOutRecord = new QAction(tr("车辆进出场"), this);
//    m_actInOutRecord->setToolTip(tr("车辆进出场"));
//    m_menuFunction->addAction( m_actInOutRecord );
//    connect(m_actInOutRecord, SIGNAL(triggered()), this, SLOT( slotInOut( ) ));

//    m_actShowMessage = new QAction(tr("信息显示"), this);
//    m_menuFunction->addAction( m_actShowMessage );
//    connect(m_actShowMessage, SIGNAL(triggered()), this, SLOT(slotShowMessage()));

    m_actExit = new QAction(tr("退出"), this);
    m_actExit->setToolTip(tr("退出"));
    m_actExit->setShortcut( QKeySequence(tr("Ctrl+Q")) );
//    m_actExit->setIcon(QIcon(":/images/exit.png"));
    m_menuFunction->addAction(m_actExit);
    connect(m_actExit, SIGNAL(triggered()), this, SLOT(close()));

    //帮助菜单
    m_menuHelp = menuBar()->addMenu(tr("&H帮助"));

    m_actAbout = new QAction(tr("关于..."), this);
    m_actAbout->setToolTip(tr("关于..."));
//    m_actAbout->setIcon(QIcon(":/images/about.png"));
    m_menuHelp->addAction(m_actAbout);
    connect(m_actAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));

}

////建立工具栏
//void QLdcl::createToolBar( )
//{
//    setIconSize( QSize(64, 64) ); //更改图标的大小

//    QToolBar * toolBar= addToolBar("Tool");

//    addToolBar( Qt::LeftToolBarArea, toolBar );

////    toolBar->setOrientation( Qt::Vertical );

//    QToolButton * pToolButtonVehicle = new QToolButton ;
//    pToolButtonVehicle->setIcon(QIcon(":/images/vehicle.png"));
//    toolBar->addWidget( pToolButtonVehicle );
//    pToolButtonVehicle->setToolTip ( tr("车辆登记") );
//    connect(pToolButtonVehicle,SIGNAL(clicked()),this,SLOT( slotVehicle()));

//    QToolButton * pToolButtonDriver = new QToolButton ;
//    pToolButtonDriver->setIcon(QIcon(":/images/driver.png"));
//    toolBar->addWidget( pToolButtonDriver );
//    pToolButtonDriver->setToolTip ( tr("司机登记") );
//    connect( pToolButtonDriver, SIGNAL(clicked()),this,SLOT( slotDriver() ));

//    QToolButton * pToolButtonInOut = new QToolButton ;
//    pToolButtonInOut->setIcon(QIcon(":/images/inout.png"));
//    toolBar->addWidget( pToolButtonInOut );
//    pToolButtonInOut->setToolTip ( tr("出入记录") );
//    connect( pToolButtonInOut, SIGNAL(clicked()),this,SLOT( slotInOut() ) );

//    QToolButton * pToolButtonHelp = new QToolButton ;
//    pToolButtonHelp->setIcon(QIcon(":/images/help.png"));
//    toolBar->addWidget( pToolButtonHelp );
//    pToolButtonHelp->setToolTip ( tr("帮助") );
//    connect( pToolButtonHelp, SIGNAL(clicked()),this,SLOT( slotAbout()));

////    toolBar->addWidget( pToolButtonVehicle );
////    toolBar->addWidget( pToolButtonDriver );
////    toolBar->addWidget( pToolButtonInOut );
////    toolBar->addWidget( pToolButtonHelp );
//}


//联接数据库
void QLdcl::connectDatabase()
{
    QSqlDatabase m_mainDB = QSqlDatabase::addDatabase("QSQLITE" );          //连接数据库
    QString strPath = QCoreApplication::applicationDirPath(  );

    m_mainDB.setDatabaseName( strPath + "/ldcl.cfg");

    bool bRes = m_mainDB.open( );

    if (!bRes) {
        QMessageBox::warning(NULL, tr("连接数据库失败！"), tr("连接数据库失败！"));
        return;
    }
}

void QLdcl::slotAbout()
{
    QMessageBox msgBox ;
    msgBox.about(this, tr("关于..."), tr("缆道测速系统 \n ver1.000"));
}

//断面设置
void QLdcl::slotConfigDm( )
{
    closeWin( );

    QWidget *  pSubWidget = new QWidgetCxE( QString("dm") );

    m_mdiArea->addSubWindow( pSubWidget );

    pSubWidget->showMaximized( );
    pSubWidget->show( );
}

//程序配置
void QLdcl::slotSysconfig( )
{
    closeWin( );

    QWidget *  pSubWidget = new QWidgetCxE( QString("sysconfig") );

    m_mdiArea->addSubWindow( pSubWidget );

    pSubWidget->showMaximized( );
    pSubWidget->show( );
}

//缆道流速测量
void QLdcl::slotCl( )
{
    closeWin( );

    QWidget * pSubWidget = new QWidgetCl;

    m_mdiArea->addSubWindow( pSubWidget );
    pSubWidget->showMaximized( );
    pSubWidget->show( );
}

void QLdcl::closeWin( )
{
    if (m_mdiArea->currentSubWindow()) {
        m_mdiArea->currentSubWindow()->close();
    }
}
