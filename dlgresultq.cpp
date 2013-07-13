#include "dlgresultq.h"
#include "combodelegate.h"
#include "func.h"
#include "getdm.h"

#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QDateTime>
#include <QFont>
#include <QFrame>

QDlgResultQ::QDlgResultQ( QWidget* parent, Qt::WindowFlags flags )
    : QDialog(parent, flags)
{

    lblStart = new QLabel(tr("��ʼʱ�䣺")) ;
    lblEnd = new QLabel(tr("����ʱ�䣺")) ;

    edtDateStart = new QLineEdit  ;
    edtDateEnd = new QLineEdit ;

    buttonQueryCl = new QPushButton(tr("���������ѯ")) ;
    connect(buttonQueryCl, SIGNAL(clicked()), this, SLOT(slotQueryCl()));

    checkBoxSelect = new  QCheckBox(tr("ȫѡ")) ;
    connect(checkBoxSelect, SIGNAL( stateChanged(int) ), this, SLOT(slotSelectChange(int)));

    QHBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget( lblStart );
    layout1->addWidget( edtDateStart );

    QHBoxLayout *layout2 = new QHBoxLayout;
    layout2->addWidget( lblEnd );
    layout2->addWidget( edtDateEnd );

    QHBoxLayout *layoutButtonCl = new QHBoxLayout;
//    layoutButtonCl->addStretch( );
    layoutButtonCl->addWidget( checkBoxSelect );
    layoutButtonCl->addWidget( buttonQueryCl );

    pTableCl = new QTableWidget ;
    pTableQ = new QTableWidget ;

    QFrame * leftFrame =new QFrame ;

    leftFrame->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    leftFrame->setMinimumWidth( 400 );

    QVBoxLayout *layoutLeft = new QVBoxLayout( leftFrame );
    layoutLeft->addLayout( layout1 );
    layoutLeft->addLayout( layout2 );
    layoutLeft->addLayout( layoutButtonCl );
    layoutLeft->addWidget(pTableCl);


    lblGcStart = new QLabel(tr("��ʼˮ��߳�:")) ;
    lblGcEnd = new QLabel(tr("����ˮ��̣߳�")) ;

    edtGcStart = new QLineEdit  ;
    edtGcEnd = new QLineEdit ;

    buttonMakeQ =  new QPushButton(tr("����������")) ;
    connect(buttonMakeQ, SIGNAL(clicked()), this, SLOT(slotMakeQ()));

    buttonMakeReport =  new QPushButton(tr("���ɱ���")) ;
    connect(buttonMakeReport, SIGNAL(clicked()), this, SLOT(slotMakeReport()));

    QHBoxLayout *layoutGc = new QHBoxLayout;
    layoutGc->addWidget( lblGcStart );
    layoutGc->addWidget( edtGcStart );
    layoutGc->addWidget( lblGcEnd );
    layoutGc->addWidget( edtGcEnd );

    QHBoxLayout *layoutMake = new QHBoxLayout;
    layoutMake->addWidget( buttonMakeQ );
    layoutMake->addWidget( buttonMakeReport );

    QVBoxLayout *layoutRight = new QVBoxLayout;
    layoutRight->addLayout( layoutGc );
    layoutRight->addLayout( layoutMake );
    layoutRight->addWidget( pTableQ );

    QHBoxLayout *mainLayout = new QHBoxLayout;
//    mainLayout->addLayout( layoutLeft );
    mainLayout->addWidget( leftFrame );
    mainLayout->addLayout( layoutRight );

    setLayout( mainLayout );

    resize(1200,600);

    setWindowTitle(tr("��������"));

    //�������
    QDateTime dtToday = QDateTime::currentDateTime( );
    QDate d = dtToday.date() ;
    QString strTime1, strTime2 ;

    strTime1.sprintf( "%04d-%02d-%02d 00:00:00", d.year(),d.month(),d.day() );
    strTime2.sprintf( "%04d-%02d-%02d 23:59:59", d.year(),d.month(),d.day() );
    edtDateStart->setText( strTime1 );
    edtDateEnd->setText( strTime2 );

    QFunc  func ;
    QString strGcStart , strGcEnd ;
    strGcStart = func.getSysconfig( 1 );
    strGcEnd = func.getSysconfig( 2 );
    edtGcStart->setText( strGcStart );
    edtGcEnd->setText( strGcEnd );

    pTableCl->setColumnCount( 4 );
    QStringList listTitle ;
    listTitle << tr(" ") << tr("ʱ��") << tr("����") << tr("����") ;
    pTableCl->setHorizontalHeaderLabels(listTitle);
    pTableCl->setColumnWidth(0, 20);
    pTableCl->setColumnWidth(1, 170);
    pTableCl->setColumnWidth(2, 80);
    pTableCl->setColumnWidth(3, 80);

    pTableCl->setFont( QFont("ZYSong18030",12) );

    pTableCl->verticalHeader()->setVisible(false);   //�����б�ͷ
    pTableQ->verticalHeader()->setVisible(false);   //�����б�ͷ

    setTitleQ(  );

//    sslr( 12.62, 1 );
//    sslr( 12.65, 1 );
//    sslr( 12.66, 1 );
//    sslr( 12.666, 2 );
}

QDlgResultQ::~QDlgResultQ()
{

}



//��ʽ��ʱ�䴮
QString QDlgResultQ::formatTime( QString strTime )
{
    QStringList list1, listDate, listTime ;
    list1 = strTime.split( " " );
//    qDebug() << "list size " << list1.size() ;
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

//    qDebug( ) << str1 ;
    return str1 ;
}

//��ѯ�������
void QDlgResultQ::slotQueryCl( )
{
    QString strSQL ;

    strSQL = QString("select count(*) from result where dt > '%1' and dt < '%2' order by dt desc")
            .arg( formatTime( edtDateStart->text()) ).arg( formatTime(edtDateEnd->text()) );

    //    qDebug( ) << strSQL ;

    QSqlQuery  query ;
    query.exec( strSQL );

    int iRowCount ;
    while ( query.next() ){
        iRowCount = query.value(0).toInt( );
        pTableCl->setRowCount( iRowCount );
    }

    strSQL = QString("select dt,qdj,ls from result where dt > '%1' and dt < '%2' order by dt desc")
            .arg( formatTime( edtDateStart->text()) ).arg( formatTime(edtDateEnd->text()) );
    query.exec( strSQL );

    QDateTime dt ;
    QDate d ;
    QTime t ;
    QString strTime ;

    float fQdj, fLs ;
    int i = 0 ;

    while ( query.next() ){
        dt = query.value(0).toDateTime() ;
        fQdj = query.value(1).toFloat();
        fLs = query.value(2).toFloat();

        d = dt.date() ;
        t = dt.time() ;
        strTime.sprintf( "%04d-%02d-%02d %02d:%02d:%02d ", d.year(),d.month(),d.day(),t.hour(),t.minute(),t.second() );

        pTableCl->setItem( i ,1, new QTableWidgetItem(  QString("%1").arg( strTime )  ));
        pTableCl->setItem( i ,2, new QTableWidgetItem(  QString("%1").arg(fQdj)  ));
        pTableCl->setItem( i ,3, new QTableWidgetItem(  QString("%1").arg(fLs)  ));

        QTableWidgetItem * pCheck = new QTableWidgetItem ;
        pCheck->setCheckState( Qt::Unchecked );
        pTableCl->setItem( i ,0, pCheck );

        i ++ ;
    }
}

//����������
void QDlgResultQ::slotMakeQ( )
{
    pTableQ->clear();
    setTitleQ( );

    //�������ʱ��ˮ��߳�
    QString strSQL ;

    strSQL = QString("update sysconfig set content = '%1' where id = 2 ")
            .arg( edtGcEnd->text() );

    QSqlQuery  query ;
    query.exec( strSQL );

    getCsGc(  );
    getVGc( );
    fillArea( );
    computerLs();
    fillTable();
    tj( );
    fillTj( );
}

//��ȡ����߳�
void QDlgResultQ::getCsGc(  )
{
    //��ƽ���߳�
    float fGcStart, fGcEnd  ;
    fGcStart = edtGcStart->text().toFloat();
    fGcEnd = edtGcEnd->text().toFloat();
    fWaterGc = ( fGcStart + fGcEnd ) / 2.0 ;

    QGetDm dm ;
    dm.readDm();

    fLsParaLeft = dm.getSysconfig( 3 ).toFloat();
    fLsParaRight = dm.getSysconfig( 4 ).toFloat();
    fQdjOffset = dm.getSysconfig( 5 ).toFloat();

    int iLeft, iRight ;
    iLeft = dm.getQdjLeftIndex( fWaterGc );
    iRight = dm.getQdjRightIndex( fWaterGc );

    fQdjLeft = dm.getQdjLeft( fWaterGc ) ;
    fQdjRight = dm.getQdjRight( fWaterGc ) ;

    QString strSQL ;
    QSqlQuery  query ;

    strSQL = QString("select qdj, ss from dm where id >= %1 and id <= %2 ")
            .arg( iLeft ).arg( iRight );

    query.exec( strSQL );

    int i = 0 ;
    float fQdj, fSs, fYySs ;  //���࣬�ӵ׸̣߳�Ӧ��ˮ��

    bool bFirst = true ;

    while ( query.next() ){
        fQdj = query.value(0).toFloat();
        fSs = query.value(1).toFloat();

        pTableQ->setItem( i ,INDEX_CS_NO, new QTableWidgetItem(  QString("%1").arg( i+1 )  ));
        pTableQ->setItem( i ,INDEX_QDJ, new QTableWidgetItem( QString("%1").arg( fQdj ) ));
        pTableQ->setItem( i ,INDEX_GC, new QTableWidgetItem(  QString("%1").arg( fSs ) ));

        fYySs = fWaterGc - fSs ;  //����̼߳�ȥ�ӵ׸߳�
        if( fYySs<=0 ){
            fYySs = 0 ;
            listYySs << fYySs ;
            if( bFirst )
                {listQdj << fQdjLeft ;  bFirst = false; }
            else
                listQdj << fQdjRight ;
        }
        else{
            listQdj << fQdj ;
            listYySs << fYySs ;
        }
        pTableQ->setItem( i ,INDEX_YYSS, new QTableWidgetItem(  sslr(fYySs, 2) ));
        i ++ ;
    }
    qDebug() << "listQdj" << listQdj.size()  << listQdj;
    qDebug() << "listYySs" << listYySs.size() << listYySs;
    //��дˮ��߳�
    pTableQ->setItem( 0, INDEX_WATER_GC, new QTableWidgetItem(  QString("%1").arg(fWaterGc)  ));
    pTableQ->setItem( i-1, INDEX_WATER_GC, new QTableWidgetItem(  QString("%1").arg(fWaterGc)));
}

//���ò���������Ϣ
void QDlgResultQ::getVGc( )
{
    int i , j = 1 ;
    QTableWidgetItem *  pItem ;

    for( i=pTableCl->rowCount()-1; i>=0; i-- ){
        pItem = pTableCl->item( i, 0 );
        if( pItem->checkState() == Qt::Checked ){
            pItem = pTableCl->item( i, 2 );
            fillVNo( pItem->text( ), j,  pTableCl->item( i, 3 )->text() );
            j++ ;
        }
    }

    //����������ţ����ϵ��°� 1 2 3 ... ����
    j = 1;
    for( i = 0; i < pTableQ->rowCount(); i++ ){
        pItem = pTableQ->item( i, INDEX_CV_NO );
        if( pItem == NULL ) continue ;
        if( !pItem->text().isEmpty() ){
            pTableQ->setItem( i, INDEX_CV_NO, new QTableWidgetItem( QString("%1").arg(j) ) );
            listLs << pTableQ->item( i, INDEX_LS )->text().toFloat( ) ;
            listLsIndex << i ;
            j++ ;
        }
    }

    qDebug() << "listLs" << listLs.size() << listLs ;
    qDebug() << "listLsIndex" << listLsIndex.size() << listLsIndex ;
}

//������������Ϣ�������������Ϣ
void QDlgResultQ::fillVNo( QString strQdj, int iNo, QString strLs )
{
    int i;
    QTableWidgetItem *  pItem ;

    QString strQdjOffset ;
    float f;
    f = strQdj.toFloat( ) + fQdjOffset;

    QString strDest = QString("%1").arg(f) ;

    for( i = 0; i < pTableQ->rowCount(); i++ ){
        pItem = pTableQ->item( i, INDEX_QDJ );
        if( pItem == NULL ) continue ;
        if( pItem->text() == strDest ){
            pTableQ->setItem( i, INDEX_CV_NO, new QTableWidgetItem( QString("%1").arg(iNo) ) );
            pTableQ->setItem( i, INDEX_LS, new QTableWidgetItem( sslr(strLs.toFloat(), 2) ) );
        }
    }
}

//������
void QDlgResultQ::fillArea( )
{
    float fAverSs, fAverJj, fArea ;
    for( int i=0; i<listYySs.size()-1; i++){
        fAverSs = (listYySs.at(i) + listYySs.at(i+1)) / 2.0 ;
        fAverJj = ( listQdj.at(i+1) - listQdj.at(i)) ;
        fArea = fAverSs * fAverJj ;
        listSsArea << fArea ;
        pTableQ->setItem( i+1, INDEX_PJSS,new QTableWidgetItem( sslr(fAverSs, 2) ));
        pTableQ->setItem( i+1, INDEX_JJ,new QTableWidgetItem( sslr(fAverJj, 2) ));
        pTableQ->setItem( i+1, INDEX_QXMJ,new QTableWidgetItem( sslr(fArea, 2) ));
    }
    qDebug() << "listSsArea"  << listSsArea.size() << listSsArea;
}

//����ƽ��������������ٵ����
void QDlgResultQ::computerLs( )
{
    //�����ٶ�
    if( listLs.size() == 0 ) return ;
    listLsPj << listLs.at(0) * fLsParaLeft ;    //��ߵ�����ƽ��

    for(int i=0; i<listLs.size()-1; i++ )
        listLsPj << (listLs.at(i) +listLs.at(i+1)) / 2 ;

    listLsPj << listLs.at(listLs.size()-1) * fLsParaRight;    //�ұߵ�����ƽ��

    qDebug( ) << "listLsPj" << listLsPj.size() << listLsPj ;

    //����������� ͷ��
    if( listLsIndex.size() == 0 ) return ;

    int i;
    float fSum = 0 ;
    for( i=0; i<listLsIndex.at(0); i++ ){
        fSum += listSsArea.at(i);
    }
    listLsArea << fSum ;

    //����������� �в�
    int j, iNums;
    for( i=0; i<listLsIndex.count()-1 ; i++ ){
        iNums = listLsIndex.at(i+1) - listLsIndex.at(i) ;
        fSum = 0 ;
        for(j=0; j<iNums; j++ )
            fSum +=  listSsArea.at( listLsIndex.at(i) + j );
        listLsArea << fSum ;
    }

    //����������� β��
    int iEndStart = listLsIndex.last();
    int iEndEnd = listSsArea.size( );

    fSum = 0;
    for( i=iEndStart; i<iEndEnd; i++ ){
        fSum += listSsArea.at(i);
    }
    listLsArea << fSum ;

    qDebug( ) << "listLsArea" << listLsArea.size() << listLsArea ;

    //����ֲ�����
    float fQ ;
    for( i=0; i<listLsArea.size(); i++ ){
        fQ = listLsPj.at(i) * listLsArea.at(i) ;
        listLsQ << fQ ;
    }
    qDebug( ) << "listLsQ" << listLsQ.size() << listLsQ ;
}

//����xls����
void QDlgResultQ::slotMakeReport( )
{
    QFunc  func ;
    func.saveXLS( pTableQ );
}

//ȫѡ��״̬�ı�
void QDlgResultQ::slotSelectChange( int iSelect )
{
//    qDebug( ) << iSelect ;

    int i, iRowCount ;
    QTableWidgetItem *  pItem ;

    iRowCount = pTableCl->rowCount() ;

    for( i=0; i<iRowCount; i++ ){
        pItem = pTableCl->item( i, 0 );
        if(iSelect == 0)
            pItem->setCheckState( Qt::Unchecked );
        else
            pItem->setCheckState( Qt::Checked );
    }
}


void QDlgResultQ::setTitleQ(  )
{
    pTableQ->setColumnCount(13);
    pTableQ->setRowCount( 30 );

    QStringList list;
    list << tr("�����") << tr("���ٺ�") <<tr("����") <<  tr("ˮ��߳�") << tr("�ӵ׸߳�") ;
    list << tr("Ӧ��ˮ��") << tr("ƽ��ˮ��") << tr("����߼��") << tr("����߼����") ;
    list << tr("�������") << tr("��������") << tr("��������") << tr("��������") ;

    pTableQ->setHorizontalHeaderLabels( list );

    pTableQ->setColumnWidth(0, 60);
    pTableQ->setColumnWidth(1, 60);
    pTableQ->setColumnWidth(2, 90);

    listYySs.clear();
    listQdj.clear() ;
    listLs.clear() ;

    listSsArea.clear() ;
    listLsIndex.clear() ;
    listLsPj.clear() ;
    listLsArea.clear() ;
    listLsQ.clear() ;
}

//��������
void  QDlgResultQ::fillTable( )
{
    if( listLsIndex.size() == 0 ) return ;

    int i;

    for(i=0; i<listLsIndex.size(); i++){
        pTableQ->setItem( listLsIndex.at(i), INDEX_LSAREA,  \
                          new QTableWidgetItem( sslr(listLsArea.at(i), 2 ) ));
        pTableQ->setItem( listLsIndex.at(i), INDEX_LSPJ,  \
                          new QTableWidgetItem( sslr(listLsPj.at(i), 2 ) ));
        pTableQ->setItem( listLsIndex.at(i), INDEX_LSQ,  \
                          new QTableWidgetItem( sslr(listLsQ.at(i), 2 ) ));
    }
    pTableQ->setItem( listLsIndex.last()+1, INDEX_LSAREA,  \
                      new QTableWidgetItem( sslr(listLsArea.last( ), 2 ) ) );
    pTableQ->setItem( listLsIndex.last()+1, INDEX_LSPJ,  \
                      new QTableWidgetItem( sslr(listLsPj.last( ), 2 ) ) );
    pTableQ->setItem( listLsIndex.last()+1, INDEX_LSQ,  \
                      new QTableWidgetItem( sslr(listLsQ.last( ), 2 ) ) );
}

//��������
// fValue ת�����ֵ
// iDigit ת����С��������Ŀ
QString QDlgResultQ::sslr( float fValue, int iDigit )
{
    float f1 = 1 ;
    int i;
    for(i=0; i<iDigit; i++){
        f1 *= 10 ;
    }

    float f2 ;
    f2 = fValue * f1 ;
    f2 += (float)0.4 ;         //����������0.4����ȥ

    int iValue = (int) f2 ;

    QString strValue;
    char chFormat[30];
    sprintf( chFormat, "%%.%df", iDigit );
    strValue.sprintf( chFormat , iValue / f1 );
//    qDebug( ) << "sslr" << fValue  << strValue ;
    return strValue ;
}

//ͳ��ֵ
void QDlgResultQ::tj(  )
{
    float fSum ;
    int i;

    fSum = 0;
    for(i=0; i<listLsQ.size(); i++){
        fSum += listLsQ.at(i) ;
    }
    fTj[0] = fSum ;             //��������

    fSum = 0;
    for(i=0; i<listLsArea.size(); i++){
        fSum += listLsArea.at(i) ;
    }
    fTj[1] = fSum ;              // �������

    fSum = 0;
    for(i=0; i<listLsPj.size(); i++){
        fSum += listLsPj.at(i) ;
    }
    fTj[2] = fSum / listLsPj.size();   // ����ƽ��

    float fMax = 0 ;
    for(i=0; i<listLs.size(); i++){
        if(  fMax < listLs.at(i) ) fMax = listLs.at(i);
    }
    fTj[3] = fMax ;                     //���������
    fTj[4] = fQdjRight - fQdjLeft ;     //ˮ���

    fSum = 0;
    for(i=0; i<listYySs.size(); i++){
        fSum += listYySs.at(i) ;
    }
    fTj[5] = fSum / listYySs.size();   //ƽ��ˮ��

    fMax = 0 ;
    for(i=0; i<listYySs.size(); i++){
        if(  fMax < listYySs.at(i) ) fMax = listYySs.at(i);
    }
    fTj[6] = fMax ;     //���ˮ��
}

void QDlgResultQ::fillTj( )
{
    int iStartRow = listYySs.size() + 3 ;

    pTableQ->setItem( iStartRow, 3, new QTableWidgetItem(  tr("��������") ));
    pTableQ->setItem( iStartRow, 4, new QTableWidgetItem(  sslr(fTj[0], 2) ));
    pTableQ->setItem( iStartRow, 5, new QTableWidgetItem(  tr("�������") ));
    pTableQ->setItem( iStartRow, 6, new QTableWidgetItem(  sslr(fTj[1], 2) ));
    pTableQ->setItem( iStartRow, 7, new QTableWidgetItem(  tr("����ƽ��") ));
    pTableQ->setItem( iStartRow, 8, new QTableWidgetItem(  sslr(fTj[2], 2) ));

    pTableQ->setItem( iStartRow+1, 3, new QTableWidgetItem(  tr("���������") ));
    pTableQ->setItem( iStartRow+1, 4, new QTableWidgetItem(  sslr(fTj[3], 2) ));
    pTableQ->setItem( iStartRow+1, 5, new QTableWidgetItem(  tr("ˮ���") ));
    pTableQ->setItem( iStartRow+1, 6, new QTableWidgetItem(  sslr(fTj[4], 2) ));
    pTableQ->setItem( iStartRow+1, 7, new QTableWidgetItem(  tr("ƽ��ˮ��") ));
    pTableQ->setItem( iStartRow+1, 8, new QTableWidgetItem(  sslr(fTj[5], 2) ));

    pTableQ->setItem( iStartRow+2, 3, new QTableWidgetItem(  tr("���ˮ��") ));
    pTableQ->setItem( iStartRow+2, 4, new QTableWidgetItem(  sslr(fTj[6], 2) ));
}
