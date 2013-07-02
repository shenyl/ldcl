#include   "widgetcxe.h"
#include   "combodelegate.h"

#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>

QWidgetCxE::QWidgetCxE( QString strTabName, QWidget* parent, Qt::WindowFlags flags )
    : QWidget(parent, flags)
{
    this->strTabName = strTabName ;
    tabModel = new QSqlTableModel ;
    tabModel->setTable( strTabName );

    QSqlQuery query;
    QString strSql ;
    int i = 0 ;

    strSql = QString( "select fieldtitle from customtab where tablename = '%1' order by id " )
            .arg( strTabName );
    query.exec( strSql );


    QString strTitle ;
    while ( query.next() ){
        strTitle = query.value(0).toString();
        tabModel->setHeaderData( i , Qt::Horizontal, strTitle );
        i++ ;
    }

    tabModel->setSort(0, Qt::AscendingOrder);     //以 id 排序
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    tabView = new QTableView;
    tabView->setModel(tabModel);                                    //连接视图和模型
    tabView->setSelectionMode(QAbstractItemView::SingleSelection);
    tabView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabView->verticalHeader()->setVisible(false);

    if (strTabName == tr("cx") )
        connect( tabView, SIGNAL( pressed( QModelIndex )), \
                        this, SLOT( getFloat( QModelIndex  ) ) );

    strSql = QString( "select displaywidth, editkind, edit2 from customtab where tablename = '%1' order by id " )
            .arg( strTabName );
    query.exec( strSql );

    int iWidth, iEditKind ;
    QString strEdit2 ;

    i = 0 ;
    while ( query.next() ){
        iWidth = query.value(0).toInt( );
        iEditKind = query.value(1).toInt( );
        strEdit2 = query.value(2).toString();
        tabView->setColumnWidth( i, iWidth );

        if( iEditKind == 1 ){
            ComboDelegate * pDelegate = new ComboDelegate( iEditKind, strEdit2 );
            tabView->setItemDelegateForColumn( i, pDelegate );
        }

        i++ ;
//        qDebug() << iWidth ;
    }

    retrieveTableView(  );

//    tabView -> setEditTriggers( QAbstractItemView::DoubleClicked );
    tabView->setEditTriggers( QAbstractItemView::CurrentChanged );
//    tabView->setEditTriggers( QAbstractItemView::SelectedClicked );


    tabView->horizontalHeader()->setStretchLastSection( true ); //最后一列占满可视空间

    tabView->setCurrentIndex(tabModel->index(-1, 0));         //设置当前可编辑行

    buttonInsert = new QPushButton(tr("增加&A"));            //设置功能按钮
    buttonDelete = new QPushButton(tr("删除&D"));
    buttonUpdate = new QPushButton(tr("保存&S"));
//    buttonClose = new QPushButton(tr("关闭&C"));

    connect(buttonInsert, SIGNAL(clicked()), this, SLOT(insertRow()));
    connect(buttonDelete, SIGNAL(clicked()), this, SLOT(deleteRow()));
    connect(buttonUpdate, SIGNAL(clicked()), this, SLOT(updateRow()));
//    connect(buttonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch( );
    bottomLayout -> addWidget(buttonInsert);
    bottomLayout -> addWidget(buttonDelete);
    bottomLayout -> addWidget(buttonUpdate);
//    bottomLayout -> addWidget(buttonClose);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget( tabView );
    mainLayout -> addLayout( bottomLayout );

    setLayout( mainLayout );



}

QWidgetCxE::~QWidgetCxE()
{

}

void QWidgetCxE::retrieveTableView()
{
    tabModel->select( );
}

void QWidgetCxE::insertRow()
{
    int row = tabModel->rowCount();
    tabModel->insertRow(row);
//  tabModel->setData( tabModel->index(row, 0), QVariant(iBoxid) );  //给boxid 字段置初值

    QModelIndex index = tabModel->index(row, 0);   //设置当前行添加到行尾并定位改行可编辑
    tabView->setCurrentIndex(index);
    tabView->edit( index );

}

void QWidgetCxE::deleteRow()
{
    QModelIndex index = tabView->currentIndex( );
    if(index.isValid())
    {
        tabModel->removeRow(index.row());
        tabModel->submitAll();
        retrieveTableView();
    }
}

void QWidgetCxE::updateRow()
{
    tabView->selectRow( 0 );
    tabView->selectRow( -1 );
    tabModel->submitAll( );
    retrieveTableView( );
}


void QWidgetCxE::getFloat( QModelIndex index )
{
    QSqlRecord record = tabModel->record(index.row());
    float fQdj = record.value("qdj").toFloat( );
    float fSs = record.value("ss").toFloat( );

    qDebug( ) << fQdj << fSs ;

    emit sigQdjSS( fQdj, fSs );
}
