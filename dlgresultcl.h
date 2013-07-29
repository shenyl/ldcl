#ifndef QDlgResultCl_H
#define QDlgResultCl_H

#include <QMdiSubWindow>

#include <QDialog>
#include <QSqlTableModel>
#include <QPushButton>
#include <QTableView>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QSqlQueryModel>

//使第一二列不可编辑  时间与起点距
class QSqlTableModelResult : public QSqlTableModel
{
    Q_OBJECT
public:
    QSqlTableModelResult( QObject * parent = 0, QSqlDatabase db = QSqlDatabase() ):
        QSqlTableModel( parent, db ){ }
    Qt::ItemFlags flags(const QModelIndex &index) const
    {
        int iTemp ;
        iTemp = ~Qt::ItemIsEditable ;
        Qt::ItemFlags flags = QSqlTableModel::flags(index);
        if ( index.column() == 0 || index.column() == 1 ){
            flags &= iTemp;
        }
        return flags ;
    }
};


//结果查看类 使用QSqlTableModel类
class QDlgResultCl : public QDialog
{
    Q_OBJECT
public:
    QDlgResultCl( QString strTabName, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~QDlgResultCl( );

    QString formatTime( QString strTime );

private slots:
    void insertRow( );
    void deleteRow( );
    void updateRow( );
    void slotQuery( );
    void slotSaveXls( );

private:
    void retrieveTableView( );
    void setTitle( );

//    QSqlTableModel *tabModel ;
    QSqlTableModelResult *tabModel ;
    QTableView *tabView ;

    QLabel * lblStart, * lblEnd ;
    QLineEdit * edtStart, * edtEnd ;
    QPushButton *buttonQuery ;

    QPushButton *buttonInsert ;
    QPushButton *buttonDelete ;
    QPushButton *buttonUpdate ;
    QPushButton *buttonClose ;
    QPushButton *buttonSaveXls ;

    QString strTabName ;

    QSqlQueryModel * modelQuery ;

};

#endif  // QDlgResultCl_H
