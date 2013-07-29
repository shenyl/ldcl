#include   "widgetcxe.h"
#include   "combodelegate.h"
#include   "floatdelegate.h"

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

        if( iEditKind == 2 ){
            FloatDelegate * pDelegate = new FloatDelegate( iEditKind, strEdit2 );
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

    if (strTabName == tr("cx") ){
        pLabelGc = new QLabel( tr("水面高程") );
        edtWaterDepth = new QLineEdit ;
        buttonWaterDepth = new QPushButton(tr("计算垂线水深&W"));
        connect(buttonWaterDepth, SIGNAL(clicked()), this, SLOT(getWaterDepth()));

        getdm.readDm();
        QString  strWaterSurface ;
        strWaterSurface = getdm.getSysconfig( 1 ); //1 是水面高程的内容
        edtWaterDepth->setText( strWaterSurface );

//        fWaterGc = strWaterSurface.toFloat( );

//        float fQdj ;
//        fQdj = getdm.getQdj(  1241.47, 0 );
//        qDebug( ) << "qdj :" << fQdj ;

//        fQdj = getdm.getQdj(  1241.47, 1 );
//        qDebug( ) << "qdj :" << fQdj ;
    }

    buttonInsert = new QPushButton(tr("增加&A"));            //设置功能按钮
    buttonDelete = new QPushButton(tr("删除&D"));
    buttonUpdate = new QPushButton(tr("保存&S"));
//    buttonClose = new QPushButton(tr("关闭&C"));


    connect(buttonInsert, SIGNAL(clicked()), this, SLOT(insertRow()));
    connect(buttonDelete, SIGNAL(clicked()), this, SLOT(deleteRow()));
    connect(buttonUpdate, SIGNAL(clicked()), this, SLOT(updateRow()));
//    connect(buttonClose, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *bottomLayout = new QHBoxLayout;

    if( strTabName == tr("cx") ){
        bottomLayout -> addWidget(pLabelGc);
        bottomLayout -> addWidget(edtWaterDepth);
        bottomLayout -> addWidget(buttonWaterDepth);
    }
//    bottomLayout->addStretch( );
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

//    qDebug( ) << fQdj << fSs ;

    emit sigQdjSS( fQdj, fSs );
}

//获取垂线的水深，由水面的高程减去垂线底部的高程
void QWidgetCxE::getWaterDepth(  )
{
    float fSs , fQdj;

    int i, iRowCount ;
    iRowCount = tabModel->rowCount( );

    QSqlRecord record ;

    for(i=0; i<iRowCount; i ++ ){
        record = tabModel->record( i );
        fQdj = record.value("qdj").toFloat( );
        fSs = getdm.getSs( edtWaterDepth->text().toFloat(), fQdj );
        tabModel->setData( tabModel->index( i, 2 ), QVariant( fSs ) );
    }

    //修改系统编号为1的值
    QSqlQuery query;
    QString strSql ;
    strSql = QString( "update sysconfig set content = '%1' where id = 1" ).arg( edtWaterDepth->text() ) ;
    query.exec( strSql );

    emit sigWaterGc( edtWaterDepth->text() );

//    fWaterGc = .toFloat();

}
