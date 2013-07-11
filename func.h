#ifndef FUNC_H
#define FUNC_H

#include <QObject>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QString>
#include <QTableWidget>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQueryModel>

class QFunc : public QObject
{
    Q_OBJECT
public:
    explicit QFunc(QObject *parent = 0);

    void connectDatabase( int iAppid );

    void saveXLS( QTableWidget * pTable, QStringList listLen );

    void saveXLS( QTableView * pTable, QSqlQueryModel * tabModel  );

    QString getSysconfig( int iId );

private:
    QSqlDatabase m_mainDB ;



};

#endif // FUNC_H
