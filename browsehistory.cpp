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
    QObject::connect( pListView ,SIGNAL(clicked(QModelIndex)), this,
                     SLOT(slotGetIndex(QModelIndex)));


    buttonOpen = new QPushButton(tr("打开报表")) ;
    connect(buttonOpen, SIGNAL(clicked()), this, SLOT(slotOpenXls()));

    buttonClose = new QPushButton(tr("关闭")) ;
    connect(buttonClose, SIGNAL(clicked()), this, SLOT(accept()));

    QHBoxLayout *layoutButton = new QHBoxLayout ;
    layoutButton->addStretch( );
    layoutButton->addWidget( buttonOpen );
    layoutButton->addWidget( buttonClose );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget( pListView );
    mainLayout->addLayout( layoutButton );
    setLayout(mainLayout);

    resize( 500, 600  );
    setWindowTitle(tr("历史报表查询"));
}

void QBrowseHistory::slotOpenXls( QModelIndex  modelIndex )
{
    qDebug() << fileModel.filePath( modelIndex );

    QDesktopServices  ds ;
    ds ;        //去掉会有一行报警
    ds.openUrl( QUrl( fileModel.filePath( modelIndex ) ));
}

void QBrowseHistory::slotOpenXls( )
{
    qDebug() << fileModel.filePath( index );

    QDesktopServices  ds ;
    ds ;        //去掉会有一行报警
    ds.openUrl( QUrl( fileModel.filePath( index ) ));
}

void QBrowseHistory::slotGetIndex( QModelIndex  index )
{
    this->index = index ;
}
