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


    buttonOpen = new QPushButton(tr("�򿪱���")) ;
    connect(buttonOpen, SIGNAL(clicked()), this, SLOT(slotOpenXls()));

    buttonClose = new QPushButton(tr("�ر�")) ;
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
    setWindowTitle(tr("��ʷ�����ѯ"));
}

void QBrowseHistory::slotOpenXls( QModelIndex  modelIndex )
{
    qDebug() << fileModel.filePath( modelIndex );

    QDesktopServices  ds ;
    ds ;        //ȥ������һ�б���
    ds.openUrl( QUrl( fileModel.filePath( modelIndex ) ));
}

void QBrowseHistory::slotOpenXls( )
{
    qDebug() << fileModel.filePath( index );

    QDesktopServices  ds ;
    ds ;        //ȥ������һ�б���
    ds.openUrl( QUrl( fileModel.filePath( index ) ));
}

void QBrowseHistory::slotGetIndex( QModelIndex  index )
{
    this->index = index ;
}
