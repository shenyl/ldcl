#ifndef BROWSEHISTORY_H
#define BROWSEHISTORY_H

#include <QDialog>
#include <QFileSystemModel>
#include <QListView>
#include <QPushButton>
#include <QModelIndex>

class QBrowseHistory : public QDialog
{
    Q_OBJECT
public:
    explicit QBrowseHistory(QWidget *parent = 0);

    QFileSystemModel fileModel ;
    QListView   * pListView ;

    QPushButton *buttonOpen ;
    QPushButton *buttonClose ;

    QModelIndex  index ;

public slots:
    void slotOpenXls( QModelIndex  modelIndex );
    void slotGetIndex( QModelIndex  modelIndex );
    void slotOpenXls( );

};

#endif // BROWSEHISTORY_H
