#ifndef QDlgResult_H
#define QDlgResult_H

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

//垂线管理类
class QDlgResult : public QDialog
{
    Q_OBJECT
public:
    QDlgResult( QString strTabName, QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~QDlgResult( );

    QString formatTime( QString strTime );

private slots:
    void insertRow( );
    void deleteRow( );
    void updateRow( );
    void slotQuery( );

private:
    void retrieveTableView();

    QSqlTableModel *tabModel ;
    QTableView *tabView ;

    QLabel * lblStart, * lblEnd ;
    QLineEdit * edtStart, * edtEnd ;
    QPushButton *buttonQuery ;

    QPushButton *buttonInsert ;
    QPushButton *buttonDelete ;
    QPushButton *buttonUpdate ;
    QPushButton *buttonClose ;

    QString strTabName ;

    QSqlQueryModel * modelQuery ;

};

#endif  // QDlgResult_H
