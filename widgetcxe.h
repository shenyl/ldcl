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

//���߹�����

class QWidgetCxE : public QWidget
{
    Q_OBJECT
public:
    QWidgetCxE( QString strTabName, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~QWidgetCxE( );

    QSqlTableModel * getTableModel( ){ return tabModel ;}

//    float fWaterGc ;    //ˮ��߳�

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
    QLineEdit  * edtWaterDepth ;    //ˮ��߳�
    QPushButton *buttonWaterDepth ;  //����Ӧ��ˮ��

    QString strTabName ;

    QGetDm  getdm ;

signals:
    void sigQdjSS( float fQdj, float fSs );
    void sigWaterGc( QString  );

};

#endif  // QWidgetCxE_H
