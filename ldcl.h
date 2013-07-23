#ifndef QLDCL_H
#define QLDCL_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSystemTrayIcon>

class QMenu ;
class QAction ;
class QMdiArea ;
class QMdiAreaBackGround ;

class QLdcl : public QMainWindow
{
    Q_OBJECT

public:
    QLdcl( int iUserPower, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~QLdcl();

    void connectDatabase();     //联接数据库

public slots:
    void slotConfigDm( );
    void slotSysconfig( );
    void slotCl( );
    void slotResultQ( );
    void slotAbout( );
    void slotBrowseHistory( );
    void slotOperators( );
    void slotLsy( );

private:
    void createMenusAndActions();


    void createToolBar( );

    void closeWin( );

private:
    QMenu* m_menuFunction;

    QAction* m_actConfigDm;
    QAction* m_actSysconfig;
    QAction* m_actCl;
    QAction* m_actResultQ;
    QAction* m_actBrowseHistory;
    QAction* m_actOperators;
    QAction* m_actLsy;


    QAction* m_actExit;

    QMenu* m_menuHelp;
    QAction* m_actAbout;

    QMdiAreaBackGround * m_mdiArea;

    QSqlDatabase m_mainDB ;
    int iUserPower ;
};

#endif  // QLDCL_H
