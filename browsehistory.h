#ifndef BROWSEHISTORY_H
#define BROWSEHISTORY_H

#include <QDialog>
#include <QFileSystemModel>
#include <QListView>

class QBrowseHistory : public QDialog
{
    Q_OBJECT
public:
    explicit QBrowseHistory(QWidget *parent = 0);

    QFileSystemModel fileModel ;
    QListView   * pListView ;


signals:

public slots:
    void slotOpenXls( QModelIndex  modelIndex );

};

#endif // BROWSEHISTORY_H
