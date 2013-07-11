#ifndef QWidgetCxE_H
#define QWidgetCxE_H

#include <QMdiSubWindow>

#include <QWidget>
#include <QSqlTableModel>
#include <QPushButton>
#include <QTableView>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include "getdm.h"

//垂线管理类

class QWidgetCxE : public QWidget
{
    Q_OBJECT
public:
    QWidgetCxE( QString strTabName, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~QWidgetCxE( );

    QSqlTableModel * getTableModel( ){ return tabModel ;}

//    float fWaterGc ;    //水面高程

private slots:
    void insertRow( );
    void deleteRow( );
    void updateRow( );
    void getFloat( QModelIndex index );
    void getWaterDepth(  ) ;

private:
    void retrieveTableView();

    QSqlTableModel *tabModel ;
    QTableView *tabView ;

    QPushButton *buttonInsert ;
    QPushButton *buttonDelete ;
    QPushButton *buttonUpdate ;
    QPushButton *buttonClose ;

    QLabel * pLabelGc ;
    QLineEdit  * edtWaterDepth ;    //水面高程
    QPushButton *buttonWaterDepth ;  //计算应用水深

    QString strTabName ;

    QGetDm  getdm ;

signals:
    void sigQdjSS( float fQdj, float fSs );
    void sigWaterGc( QString  );

};

#endif  // QWidgetCxE_H
