#include   "dlgresult.h"
#include   "combodelegate.h"

#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QDateTime>

QDlgResult::QDlgResult( QString strTabName, QWidget* parent, Qt::WindowFlags flags )
    : QDialog(parent, flags)
{
    this->strTabName = strTabName ;

    modelQuery = new QSqlQueryModel ;

    tabView = new QTableView;
    tabView->setModel( modelQuery );

    tabView->setSelectionMode(QAbstractItemView::SingleSelection);
    tabView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabView->verticalHeader()->setVisible(false);

    tabView->setEditTriggers( QAbstractItemView::NoEditTriggers );

    tabView->horizontalHeader()->setStretchLastSection( true ); //最后一列占满可视空间

    buttonInsert = new QPushButton(tr("增加&A"));            //设置功能按钮
    buttonDelete = new QPushButton(tr("删除&D"));
    buttonUpdate = new QPushButton(tr("保存&S"));
    buttonClose = new QPushButton(tr("关闭&C"));

    connect(buttonInsert, SIGNAL(clicked()), this, SLOT(insertRow()));
    connect(buttonDelete, SIGNAL(clicked()), this, SLOT(deleteRow()));
    connect(buttonUpdate, SIGNAL(clicked()), this, SLOT(updateRow()));
    connect(buttonClose, SIGNAL(clicked()), this, SLOT(close()));

    lblStart = new QLabel(tr("开始时间：")) ;
    lblEnd = new QLabel(tr("结束时间：")) ;

    edtStart = new QLineEdit  ;
    edtEnd = new QLineEdit ;
    buttonQuery = new QPushButton(tr("查询"));
    connect(buttonQuery, SIGNAL(clicked()), this, SLOT(slotQuery()));

    QDateTime dtToday = QDateTime::currentDateTime( );
    QDate d = dtToday.date() ;
    QString strTime1, strTime2 ;

    strTime1.sprintf( "%04d-%02d-%02d 00:00:00", d.year(),d.month(),d.day() );
    strTime2.sprintf( "%04d-%02d-%02d 23:59:59", d.year(),d.month(),d.day() );
    edtStart->setText( strTime1 );
    edtEnd->setText( strTime2 );

    QHBoxLayout *queryLayout = new QHBoxLayout;
    queryLayout->addWidget( lblStart );
    queryLayout->addWidget( edtStart );
    queryLayout->addWidget( lblEnd );
    queryLayout->addWidget( edtEnd );
    queryLayout->addWidget( buttonQuery );

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch( );
//    bottomLayout -> addWidget(buttonInsert);
//    bottomLayout -> addWidget(buttonDelete);
//    bottomLayout -> addWidget(buttonUpdate);
    bottomLayout -> addWidget(buttonClose);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addLayout( queryLayout );
    mainLayout -> addWidget( tabView );
    mainLayout -> addLayout( bottomLayout );

    setLayout( mainLayout );

    resize(800,400);
    slotQuery( );
    setWindowTitle(tr("流速测量结果"));

}

QDlgResult::~QDlgResult()
{

}

void QDlgResult::retrieveTableView()
{
    tabModel->select( );
}

void QDlgResult::insertRow()
{
    int row = tabModel->rowCount();
    tabModel->insertRow(row);
//  tabModel->setData( tabModel->index(row, 0), QVariant(iBoxid) );  //给boxid 字段置初值

    QModelIndex index = tabModel->index(row, 0);   //设置当前行添加到行尾并定位改行可编辑
    tabView->setCurrentIndex(index);
    tabView->edit( index );

}

void QDlgResult::deleteRow()
{
    QModelIndex index = tabView->currentIndex( );
    if(index.isValid())
    {
        tabModel->removeRow(index.row());
        tabModel->submitAll();
        retrieveTableView();
    }
}

void QDlgResult::updateRow()
{
    tabView->selectRow( 0 );
    tabView->selectRow( -1 );
    tabModel->submitAll( );
    retrieveTableView( );
}

//格式化时间串
QString QDlgResult::formatTime( QString strTime )
{
    QStringList list1, listDate, listTime ;
    list1 = strTime.split( " " );
    qDebug() << "list size " << list1.size() ;
    if( list1.size() != 2 ) return strTime ;

    listDate = list1.at(0).split( "-" ) ;
    listTime = list1.at(1).split( ":" ) ;
    if( listDate.size() != 3 ) return strTime ;
    if( listTime.size() != 3 ) return strTime ;

    int iTime[6] ;
    iTime[0] = listDate.at(0).toInt();
    iTime[1] = listDate.at(1).toInt();
    iTime[2] = listDate.at(2).toInt();
    iTime[3] = listTime.at(0).toInt();
    iTime[4] = listTime.at(1).toInt();
    iTime[5] = listTime.at(2).toInt();

    QString str1 ;
    str1.sprintf( "%04d-%02d-%02d %02d:%02d:%02d", iTime[0], iTime[1], iTime[2], \
               iTime[3], iTime[4], iTime[5] );

    qDebug( ) << str1 ;
    return str1 ;
}

void QDlgResult::slotQuery( )
{
    QString strSQL ;

    strSQL = QString("select dt,qdj,ss,ls,bz from result where dt > '%1' and dt < '%2' order by dt desc")
            .arg( formatTime( edtStart->text()) ).arg( formatTime(edtEnd->text()) );

    //    qDebug( ) << strSQL ;

    modelQuery->setQuery( strSQL );

    QSqlQuery query;
    QString strSql ;
    int i = 0 ;

    strSql = QString( "select fieldtitle from customtab where tablename = '%1' order by id " )
            .arg( strTabName );
    query.exec( strSql );


    QString strTitle ;
    while ( query.next() ){
        strTitle = query.value(0).toString();
        modelQuery->setHeaderData( i , Qt::Horizontal, strTitle );
        i++ ;
    }

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

}

