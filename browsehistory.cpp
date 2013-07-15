#include "browsehistory.h"

#include <QCoreApplication>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

QBrowseHistory::QBrowseHistory(QWidget *parent) :
    QDialog(parent)
{

    QString strPath = QCoreApplication::applicationDirPath(  );
//    qDebug( ) << strPath ;
    fileModel.setRootPath( strPath );

    QStringList  listFilters ;
    listFilters << "rep*.xlsx" ;
    fileModel.setNameFilters( listFilters );
    fileModel.setNameFilterDisables( false );

    pListView = new QListView ;
    pListView->setModel( &fileModel );
    pListView->setRootIndex(fileModel.index( strPath ));

    QObject::connect( pListView ,SIGNAL(doubleClicked(QModelIndex)), this,
                     SLOT(slotOpenXls(QModelIndex)));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget( pListView );
    setLayout(mainLayout);

    resize( 500, 600  );
    setWindowTitle(tr("��ʷ�����ѯ"));
}


void QBrowseHistory::slotOpenXls( QModelIndex  modelIndex )
{
    qDebug() << fileModel.filePath( modelIndex );

    QDesktopServices  ds ;
    ds ;        //ȥ������һ�б���
    ds.openUrl( QUrl( fileModel.filePath( modelIndex ) ));

}
