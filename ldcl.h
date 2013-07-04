#ifndef QLDCL_H
#define QLDCL_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSystemTrayIcon>
//aaaaaa

class QMenu ;
class QAction ;
class QMdiArea ;
class QMdiAreaBackGround ;

class QLdcl : public QMainWindow
{
    Q_OBJECT

public:
    QLdcl(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~QLdcl();

public slots:
    void slotConfigDm( );
    void slotSysconfig( );
    void slotCl( );
    void slotAbout( );

private:
    void createMenusAndActions();
    void connectDatabase();

//    void createToolBar( );

    void closeWin( );

private:
    QMenu* m_menuFunction;

    QAction* m_actConfigDm;
    QAction* m_actSysconfig;
    QAction* m_actCl;

    QAction* m_actExit;

    QMenu* m_menuHelp;
    QAction* m_actAbout;

    QMdiAreaBackGround * m_mdiArea;

    QSqlDatabase m_mainDB;
};

#endif  // QLDCL_H
