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

    setCentralWidget(m_mdiArea);//��Ϊ���봰�ڲ���

    connectDatabase();

    setWindowTitle(tr("�µ�����ϵͳ"));

    showMaximized( );
}

QLdcl::~QLdcl()
{

}

void QLdcl::createMenusAndActions()
{
    //���ܲ˵�
    m_menuFunction = menuBar()->addMenu(tr("&F����"));

    m_actConfigDm = new QAction(tr("��������"), this);
    m_actConfigDm->setToolTip(tr("��������"));
    m_actConfigDm->setShortcut( QKeySequence(tr("Ctrl+A")) );
    m_menuFunction->addAction(m_actConfigDm);

    connect(m_actConfigDm, SIGNAL(triggered()), this, SLOT( slotConfigDm( ) ));

    m_actSysconfig = new QAction(tr("ϵͳ����"), this);
    m_actSysconfig->setToolTip(tr("ϵͳ����"));
    m_actSysconfig->setShortcut( QKeySequence(tr("Ctrl+C")) );
    m_menuFunction->addAction(m_actSysconfig);

    connect(m_actSysconfig, SIGNAL(triggered()), this, SLOT( slotSysconfig( ) ));


    m_actCl = new QAction(tr("����"), this);
    m_actCl->setToolTip(tr("����)"));
    m_actCl->setShortcut( QKeySequence(tr("Ctrl+B")) );
    m_menuFunction->addAction(m_actCl);
    connect(m_actCl, SIGNAL(triggered()), this, SLOT(slotCl()));

//    m_actInOutRecord = new QAction(tr("����������"), this);
//    m_actInOutRecord->setToolTip(tr("����������"));
//    m_menuFunction->addAction( m_actInOutRecord );
//    connect(m_actInOutRecord, SIGNAL(triggered()), this, SLOT( slotInOut( ) ));

//    m_actShowMessage = new QAction(tr("��Ϣ��ʾ"), this);
//    m_menuFunction->addAction( m_actShowMessage );
//    connect(m_actShowMessage, SIGNAL(triggered()), this, SLOT(slotShowMessage()));

    m_actExit = new QAction(tr("�˳�"), this);
    m_actExit->setToolTip(tr("�˳�"));
    m_actExit->setShortcut( QKeySequence(tr("Ctrl+Q")) );
//    m_actExit->setIcon(QIcon(":/images/exit.png"));
    m_menuFunction->addAction(m_actExit);
    connect(m_actExit, SIGNAL(triggered()), this, SLOT(close()));

    //�����˵�
    m_menuHelp = menuBar()->addMenu(tr("&H����"));

    m_actAbout = new QAction(tr("����..."), this);
    m_actAbout->setToolTip(tr("����..."));
//    m_actAbout->setIcon(QIcon(":/images/about.png"));
    m_menuHelp->addAction(m_actAbout);
    connect(m_actAbout, SIGNAL(triggered()), this, SLOT(slotAbout()));

}

////����������
//void QLdcl::createToolBar( )
//{
//    setIconSize( QSize(64, 64) ); //����ͼ��Ĵ�С

//    QToolBar * toolBar= addToolBar("Tool");

//    addToolBar( Qt::LeftToolBarArea, toolBar );

////    toolBar->setOrientation( Qt::Vertical );

//    QToolButton * pToolButtonVehicle = new QToolButton ;
//    pToolButtonVehicle->setIcon(QIcon(":/images/vehicle.png"));
//    toolBar->addWidget( pToolButtonVehicle );
//    pToolButtonVehicle->setToolTip ( tr("�����Ǽ�") );
//    connect(pToolButtonVehicle,SIGNAL(clicked()),this,SLOT( slotVehicle()));

//    QToolButton * pToolButtonDriver = new QToolButton ;
//    pToolButtonDriver->setIcon(QIcon(":/images/driver.png"));
//    toolBar->addWidget( pToolButtonDriver );
//    pToolButtonDriver->setToolTip ( tr("˾���Ǽ�") );
//    connect( pToolButtonDriver, SIGNAL(clicked()),this,SLOT( slotDriver() ));

//    QToolButton * pToolButtonInOut = new QToolButton ;
//    pToolButtonInOut->setIcon(QIcon(":/images/inout.png"));
//    toolBar->addWidget( pToolButtonInOut );
//    pToolButtonInOut->setToolTip ( tr("�����¼") );
//    connect( pToolButtonInOut, SIGNAL(clicked()),this,SLOT( slotInOut() ) );

//    QToolButton * pToolButtonHelp = new QToolButton ;
//    pToolButtonHelp->setIcon(QIcon(":/images/help.png"));
//    toolBar->addWidget( pToolButtonHelp );
//    pToolButtonHelp->setToolTip ( tr("����") );
//    connect( pToolButtonHelp, SIGNAL(clicked()),this,SLOT( slotAbout()));

////    toolBar->addWidget( pToolButtonVehicle );
////    toolBar->addWidget( pToolButtonDriver );
////    toolBar->addWidget( pToolButtonInOut );
////    toolBar->addWidget( pToolButtonHelp );
//}


//�������ݿ�
void QLdcl::connectDatabase()
{
    QSqlDatabase m_mainDB = QSqlDatabase::addDatabase("QSQLITE" );          //�������ݿ�
    QString strPath = QCoreApplication::applicationDirPath(  );

    m_mainDB.setDatabaseName( strPath + "/ldcl.cfg");

    bool bRes = m_mainDB.open( );

    if (!bRes) {
        QMessageBox::warning(NULL, tr("�������ݿ�ʧ�ܣ�"), tr("�������ݿ�ʧ�ܣ�"));
        return;
    }
}

void QLdcl::slotAbout()
{
    QMessageBox msgBox ;
    msgBox.about(this, tr("����..."), tr("�µ�����ϵͳ \n ver1.000"));
}

//��������
void QLdcl::slotConfigDm( )
{
    closeWin( );

    QWidget *  pSubWidget = new QWidgetCxE( QString("dm") );

    m_mdiArea->addSubWindow( pSubWidget );

    pSubWidget->showMaximized( );
    pSubWidget->show( );
}

//��������
void QLdcl::slotSysconfig( )
{
    closeWin( );

    QWidget *  pSubWidget = new QWidgetCxE( QString("sysconfig") );

    m_mdiArea->addSubWindow( pSubWidget );

    pSubWidget->showMaximized( );
    pSubWidget->show( );
}

//�µ����ٲ���
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
