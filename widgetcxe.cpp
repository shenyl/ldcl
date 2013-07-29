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

    tabModel->setSort(0, Qt::AscendingOrder);     //�� id ����
    tabModel->setEditStrategy(QSqlTableModel::OnManualSubmit);

    tabView = new QTableView;
    tabView->setModel(tabModel);                                    //������ͼ��ģ��
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


    tabView->horizontalHeader()->setStretchLastSection( true ); //���һ��ռ�����ӿռ�

    tabView->setCurrentIndex(tabModel->index(-1, 0));         //���õ�ǰ�ɱ༭��

    if (strTabName == tr("cx") ){
        pLabelGc = new QLabel( tr("ˮ��߳�") );
        edtWaterDepth = new QLineEdit ;
        buttonWaterDepth = new QPushButton(tr("���㴹��ˮ��&W"));
        connect(buttonWaterDepth, SIGNAL(clicked()), this, SLOT(getWaterDepth()));

        getdm.readDm();
        QString  strWaterSurface ;
        strWaterSurface = getdm.getSysconfig( 1 ); //1 ��ˮ��̵߳�����
        edtWaterDepth->setText( strWaterSurface );

//        fWaterGc = strWaterSurface.toFloat( );

//        float fQdj ;
//        fQdj = getdm.getQdj(  1241.47, 0 );
//        qDebug( ) << "qdj :" << fQdj ;

//        fQdj = getdm.getQdj(  1241.47, 1 );
//        qDebug( ) << "qdj :" << fQdj ;
    }

    buttonInsert = new QPushButton(tr("����&A"));            //���ù��ܰ�ť
    buttonDelete = new QPushButton(tr("ɾ��&D"));
    buttonUpdate = new QPushButton(tr("����&S"));
//    buttonClose = new QPushButton(tr("�ر�&C"));


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
//  tabModel->setData( tabModel->index(row, 0), QVariant(iBoxid) );  //��boxid �ֶ��ó�ֵ

    QModelIndex index = tabModel->index(row, 0);   //���õ�ǰ����ӵ���β����λ���пɱ༭
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

//��ȡ���ߵ�ˮ���ˮ��ĸ̼߳�ȥ���ߵײ��ĸ߳�
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

    //�޸�ϵͳ���Ϊ1��ֵ
    QSqlQuery query;
    QString strSql ;
    strSql = QString( "update sysconfig set content = '%1' where id = 1" ).arg( edtWaterDepth->text() ) ;
    query.exec( strSql );

    emit sigWaterGc( edtWaterDepth->text() );

//    fWaterGc = .toFloat();

}
