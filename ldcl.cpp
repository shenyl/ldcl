#include "ldcl.h"
#include "widgetcl.h"
#include "widgetcxe.h"
#include "qmdiareabackground.h"
#include "dlgresultq.h"
#include "browsehistory.h"

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

QLdcl::QLdcl( int iUserPower, QWidget* parent, Qt::WindowFlags flags )
	: QMainWindow(parent, flags)
{
    this->iUserPower = iUserPower ;
    qDebug( ) << "user power" << iUserPower ;
    createMenusAndActions();
    createToolBar( );

    m_mdiArea = new QMdiAreaBackGround;
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setActivationOrder(QMdiArea::ActivationHistoryOrder);

    setCentralWidget(m_mdiArea);//��Ϊ���봰�ڲ���

//    connectDatabase();

    setWindowTitle(tr("ˮ���µ����ϵͳ"));

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
    m_actConfigDm->setIcon(QIcon(":/images/dm.png"));
    m_actConfigDm->setShortcut( QKeySequence(tr("Ctrl+A")) );
    m_menuFunction->addAction(m_actConfigDm);

    connect(m_actConfigDm, SIGNAL(triggered()), this, SLOT( slotConfigDm( ) ));

    m_actSysconfig = new QAction(tr("ϵͳ����"), this);
    m_actSysconfig->setToolTip(tr("ϵͳ����"));
    m_actSysconfig->setIcon(QIcon(":/images/sysconfig.png"));
    m_actSysconfig->setShortcut( QKeySequence(tr("Ctrl+C")) );
    m_menuFunction->addAction(m_actSysconfig);

    connect(m_actSysconfig, SIGNAL(triggered()), this, SLOT( slotSysconfig( ) ));

    if( iUserPower == 9 ){
        m_actOperators = new QAction(tr("��Ա����"), this);
        m_actOperators->setToolTip(tr("ϵͳ����"));
        m_actOperators->setIcon(QIcon(":/images/operators.png"));
        //    m_actOperators->setShortcut( QKeySequence(tr("Ctrl+C")) );
        m_menuFunction->addAction(m_actOperators);
        connect(m_actOperators, SIGNAL(triggered()), this, SLOT( slotOperators( ) ));
    }

    m_actLsy = new QAction(tr("����������"), this);
    m_actLsy->setToolTip(tr("����������"));
    m_menuFunction->addAction( m_actLsy );
    connect(m_actLsy, SIGNAL(triggered()), this, SLOT( slotLsy( )));

    m_actCl = new QAction(tr("����"), this);
    m_actCl->setToolTip(tr("����"));
    m_actCl->setShortcut( QKeySequence(tr("Ctrl+B")) );
    m_actCl->setIcon(QIcon(":/images/ldcl.png"));
    m_menuFunction->addAction(m_actCl);
    connect(m_actCl, SIGNAL(triggered()), this, SLOT(slotCl()));

    m_actResultQ = new QAction(tr("��������"), this);
    m_actResultQ->setToolTip(tr("��������"));
    m_actResultQ->setShortcut( QKeySequence(tr("Ctrl+D")) );
    m_actResultQ->setIcon(QIcon(":/images/lljs.png"));
    m_menuFunction->addAction(m_actResultQ);
    connect(m_actResultQ, SIGNAL(triggered()), this, SLOT(slotResultQ()));

    m_actBrowseHistory = new QAction(tr("��ʷ�����ѯ"), this);
    m_actBrowseHistory->setToolTip(tr("��ʷ�����ѯ"));
    m_actBrowseHistory->setShortcut( QKeySequence(tr("Ctrl+H")) );
    m_actBrowseHistory->setIcon(QIcon(":/images/reporthistory.png"));
    m_menuFunction->addAction(m_actBrowseHistory);
    connect(m_actBrowseHistory, SIGNAL(triggered()), this, SLOT(slotBrowseHistory()));

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

//����������
void QLdcl::createToolBar( )
{
    setIconSize( QSize(64, 64) ); //����ͼ��Ĵ�С

    QToolBar * toolBar= addToolBar( tr("������") );

    toolBar->addAction( m_actConfigDm );
    toolBar->addAction( m_actSysconfig );
    toolBar->addAction( m_actCl );
    toolBar->addAction( m_actResultQ );
    toolBar->addAction( m_actBrowseHistory );

    if( iUserPower == 9 )
        toolBar->addAction( m_actOperators );

    addToolBar( Qt::LeftToolBarArea, toolBar );
    toolBar->setOrientation( Qt::Vertical );
}


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
    msgBox.about(this, tr("����..."), tr("ˮ���µ����ϵͳ \n ver1.000"));
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

void QLdcl::slotResultQ( )
{
    QDlgResultQ  dlg ;
    dlg.exec( );
}

void QLdcl::slotBrowseHistory( )
{
    QBrowseHistory  browse ;
    browse.exec(  );
}

//����Ա����
void QLdcl::slotOperators( )
{
    closeWin( );

    QWidget *  pSubWidget = new QWidgetCxE( QString("operators") );

    m_mdiArea->addSubWindow( pSubWidget );

    pSubWidget->showMaximized( );
    pSubWidget->show( );
}

//������
void QLdcl::slotLsy( )
{
    closeWin( );

    QWidget *  pSubWidget = new QWidgetCxE( QString("lsy") );

    m_mdiArea->addSubWindow( pSubWidget );

    pSubWidget->showMaximized( );
    pSubWidget->show( );
}
