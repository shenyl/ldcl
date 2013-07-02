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

//垂线管理类

class QWidgetCxE : public QWidget
{
    Q_OBJECT
public:
    QWidgetCxE( QString strTabName, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~QWidgetCxE( );

    QSqlTableModel * getTableModel( ){ return tabModel ;}

private slots:
    void insertRow( );
    void deleteRow( );
    void updateRow( );
    void getFloat( QModelIndex index );

private:
    void retrieveTableView();

    QSqlTableModel *tabModel ;
    QTableView *tabView ;

    QPushButton *buttonInsert ;
    QPushButton *buttonDelete ;
    QPushButton *buttonUpdate ;
    QPushButton *buttonClose ;

    QString strTabName ;

signals:
    void sigQdjSS( float fQdj, float fSs );

};

#endif  // QWidgetCxE_H
