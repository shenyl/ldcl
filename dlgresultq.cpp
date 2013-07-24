#include "dlgresultq.h"
#include "combodelegate.h"
#include "func.h"
#include "getdm.h"
#include "graphicviewv.h"
#include "itemv.h"

#include <QHBoxLayout>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>
#include <QDateTime>
#include <QFont>
#include <QFrame>

#include <QtGui/QApplication>
#include <QFileDialog>
#include <QObject>
#include <QAxObject>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

QDlgResultQ::QDlgResultQ( QWidget* parent, Qt::WindowFlags flags )
    : QDialog(parent, flags)
{
    pTab = new QTabWidget ;

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
    layoutButtonCl->addWidget( checkBoxSelect );
    layoutButtonCl->addWidget( buttonQueryCl );

    pTableCl = new QTableWidget ;
    pTableQ = new QTableWidget ;
    pWidgetV = new QWidgetV ;

    pTab->addTab( pTableQ, tr("������") );
    pTab->addTab( pWidgetV, tr("�ٶ���") );

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

    buttonMakeQ =  new QPushButton(tr("���㲢����������")) ;
    connect(buttonMakeQ, SIGNAL(clicked()), this, SLOT(slotMakeQ()));

    buttonMakeReport =  new QPushButton(tr("���ɱ���")) ;
    connect(buttonMakeReport, SIGNAL(clicked()), this, SLOT(slotMakeReport()));

    buttonBrowseVLine =  new QPushButton(tr("�鿴�ٶ���")) ;
    connect(buttonBrowseVLine, SIGNAL(clicked()), this, SLOT( slotBrowseVLine()));

    labelLsy = new QLabel( tr("�������ͺ�: ") );

    InitComboLsy( );    //��ʼ��������

    QHBoxLayout *layoutGc = new QHBoxLayout;
    layoutGc->addWidget( lblGcStart );
    layoutGc->addWidget( edtGcStart );
    layoutGc->addWidget( lblGcEnd );
    layoutGc->addWidget( edtGcEnd );

    QHBoxLayout *layoutMake = new QHBoxLayout;
    layoutMake->addStretch( );

    layoutMake->addWidget( labelLsy );
    layoutMake->addWidget( pComboLsy );
    layoutMake->addWidget( buttonBrowseVLine );
    layoutMake->addWidget( buttonMakeQ );
//    layoutMake->addWidget( buttonMakeReport );

    QVBoxLayout *layoutRight = new QVBoxLayout;
    layoutRight->addLayout( layoutGc );
    layoutRight->addLayout( layoutMake );
    layoutRight->addWidget( pTab );

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
    slotQueryCl( );

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
    if( pTableCl->rowCount() == 0 ){
        QMessageBox::information(0, "", tr("����ѡ��������!") );
        return ;
    }

    pTableQ->clear( );
    setTitleQ( );

    //�������ʱ��ˮ��߳�
    QString strSQL ;

    strSQL = QString("update sysconfig set content = '%1' where id = 2 ")
            .arg( edtGcEnd->text() );

    QSqlQuery  query ;
    query.exec( strSQL );

    //���������ǵ����
    strSQL = QString("update sysconfig set content = '%1' where id = 6 ")
            .arg( pComboLsy->currentIndex() );
    query.exec( strSQL );

    getCsGc(  );
    getVGc( );
    fillArea( );
    computerLs();
    fillTable();
    tj( );
    fillTj( );
    saveXLS( );
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

    fLsParaLeft = dm.getSysconfig( 3 ).toFloat();       //����ϵ��
    fLsParaRight = dm.getSysconfig( 4 ).toFloat();
    fQdjOffset = dm.getSysconfig( 5 ).toFloat();        //����ƫ��

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

        listHdgc << fSs ;

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
        listAverSs << fAverSs ;
        listAverJj << fAverJj ;
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
//    QFunc  func ;
//    func.saveXLS( pTableQ );
    saveXLS(  );
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

//���xls�ļ�
void QDlgResultQ::saveXLS(  )
{
    //������������
    QString strPath = QCoreApplication::applicationDirPath(  );
    QString strFormatFile = strPath + "/tableformat.xlsx" ;
    strFormatFile = strFormatFile.replace("/", "\\");
    qDebug( ) << strFormatFile ;

    QDateTime dtToday = QDateTime::currentDateTime( );
    QDate d = dtToday.date() ;
    QTime t = dtToday.time() ;
    QString strTime ;
    strTime.sprintf( "%04d-%02d-%02d_%02d-%02d-%02d", d.year(),d.month(),d.day(),t.hour(),t.minute(),t.second() );

    QString strDest = strPath + "/rep" + strTime + ".xlsx" ;
    strDest = strDest.replace("/", "\\");
    qDebug( ) << strDest ;

    QAxObject excelObj("Excel.Application");
    QAxObject* excelWorkBooks = excelObj.querySubObject("Workbooks");
    QAxObject* excelWorkBook;
    QAxObject* excelSheets;

    if (excelWorkBooks) {
        QFile file(strPath);
        if (file.exists())
            excelWorkBook = excelWorkBooks->querySubObject("Open(const QString&)", strFormatFile );
        else
            excelWorkBook = excelWorkBooks->querySubObject("Add()");

        if (excelWorkBook)
            excelSheets = excelWorkBook->querySubObject("Sheets");
         else
            QMessageBox::information(0, "", "QAxObject workbook fail!");
    }
    else
        QMessageBox::information(0, "", "��ʼ��Excel����,����û�а�װOffice���!");

    QAxObject* excelWorkSheet = excelSheets->querySubObject("Item(int index)", 1);

    QAxObject* range;
    QString strText;

    int i ;
    //�����������һ����1��ʼ������
    for(i=0; i<listQdj.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartCSNo\")" );
        range = range->querySubObject("Offset( int, int )", i, 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( i+1 ));
        range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4
    }

    //���������
    for(i=0; i< listLsIndex.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartCVNo\")" );
        range = range->querySubObject("Offset( int, int )", listLsIndex.at(i), 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( i+1 ));
        range->setProperty("HorizontalAlignment", 2);    //���� 2 ���� 3  ����4
    }


    //�����б�
    for(i=0; i<listQdj.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartQdj\")" );
        range = range->querySubObject("Offset( int, int )", i, 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listQdj.at(i),1) ));
        range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4
    }

    //����߳�
    range = excelWorkSheet->querySubObject("Range(\"StartWaterGc\")" );
    range = range->querySubObject("Offset( int, int )", 0, 0 );
    range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(fWaterGc,2) ));
    range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4

    range = excelWorkSheet->querySubObject("Range(\"StartWaterGc\")" );
    range = range->querySubObject("Offset( int, int )", listQdj.size()-1, 0 );
    range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(fWaterGc,2) ));
    range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4

    //Ӧ��ˮ��
    for(i=0; i<listYySs.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartYySs\")" );
        range = range->querySubObject("Offset( int, int )", i, 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listYySs.at(i),2) ));
        range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4
    }

    //Ӧ��ˮ��ƽ��
    for(i=0; i<listAverSs.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartAverSs\")" );
        range = range->querySubObject("Offset( int, int )", i+1, 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listAverSs.at(i),2) ));
        range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4
    }

    //�����߼��
    for(i=0; i<listAverJj.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartAverJj\")" );
        range = range->querySubObject("Offset( int, int )", i+1, 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listAverJj.at(i),2) ));
        range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4
    }

    //�����߼����
    for(i=0; i<listSsArea.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartSsArea\")" );
        range = range->querySubObject("Offset( int, int )", i+1, 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listSsArea.at(i),2) ));
        range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4
    }

    //����������
    for(i=0; i<listLsIndex.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartLs\")" );
        range = range->querySubObject("Offset( int, int )", listLsIndex.at(i), 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listLs.at(i),2) ));
        range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4
    }

    //��������
    for(i=0; i<listLsIndex.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartLsPj\")" );
        range = range->querySubObject("Offset( int, int )", listLsIndex.at(i), 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listLsPj.at(i),2) ));
        range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4
    }
    range = excelWorkSheet->querySubObject("Range(\"StartLsPj\")" );
    range = range->querySubObject("Offset( int, int )", listLsIndex.last()+1, 0 );
    range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listLsPj.last(),2) ));
    range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4

    //�������
    for(i=0; i<listLsIndex.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartLsArea\")" );
        range = range->querySubObject("Offset( int, int )", listLsIndex.at(i), 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listLsArea.at(i),2) ));
        range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4
    }
    range = excelWorkSheet->querySubObject("Range(\"StartLsArea\")" );
    range = range->querySubObject("Offset( int, int )", listLsIndex.last()+1, 0 );
    range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listLsArea.last(),2) ));
    range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4

    //��������
    for(i=0; i<listLsIndex.size(); i++){
        range = excelWorkSheet->querySubObject("Range(\"StartLsQ\")" );
        range = range->querySubObject("Offset( int, int )", listLsIndex.at(i), 0 );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listLsQ.at(i),2) ));
        range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4
    }
    range = excelWorkSheet->querySubObject("Range(\"StartLsQ\")" );
    range = range->querySubObject("Offset( int, int )", listLsIndex.last()+1, 0 );
    range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr(listLsQ.last(),2) ));
    range->setProperty("HorizontalAlignment", 2);     //���� 2 ���� 3  ����4

    //���ͳ������

    char chAddr[30] ;
    for(i=0; i<7; i++){
        sprintf(chAddr, "Range(\"AddrTj%d\")", i );
        range = excelWorkSheet->querySubObject( chAddr );
        range->dynamicCall("SetValue(const QVariant&)", QVariant( sslr( fTj[i] ,2) ));
        range->setProperty("HorizontalAlignment", 4);     //���� 2 ���� 3  ����4
    }

    //����ر�excel���
    excelObj.setProperty("DisplayAlerts", 0);
    excelWorkBook->dynamicCall("SaveAs(const QString&)", strDest );
    excelWorkBook->dynamicCall("Close(Boolean)", false);

    excelObj.dynamicCall("Quit()");

    //ѯ�ʲ��������õı���
//    QMessageBox::StandardButton  buttonResult ;
    int iRes ;
    iRes = QMessageBox::question( this, tr("����"), tr("�Ƿ�������õı���?"), \
                                          QMessageBox::No, QMessageBox::Yes );

    if( iRes == QMessageBox::Yes ){
        strDest.replace( "\\" , "/" );
        QDesktopServices  ds ;
        ds ;        //ȥ������һ�б���
        ds.openUrl( QUrl( strDest ));
    }
    return ;
}

//��ʼ��������
void QDlgResultQ::InitComboLsy( )
{
    pComboLsy = new QComboBox ;

    QString strSQL ;
    strSQL = QString("select xh from lsy  order by id ");

    QSqlQuery  query ;
    query.exec( strSQL );

    QString strXh ;
    while ( query.next() ){
        strXh = query.value(0).toString();
        pComboLsy->insertItem ( pComboLsy->count(), strXh );
    }

    QString  strId ;
    strSQL = QString("select content from sysconfig where id = 6");
    query.exec( strSQL );
    while ( query.next() ){
        strId = query.value(0).toString( );
        pComboLsy->setCurrentIndex( strId.toInt() );
    }
}

//�����ٶ���
void QDlgResultQ::drawVLine( )
{
    int i ;
    float fQdj, fV ;
    QTableWidgetItem *  pItem ;

    ItemV * pItemV ;
    QGraphicViewV * pGraphicView ;
    pGraphicView = pWidgetV->pGraphicViewV ;
    pItemV = pGraphicView->getItem( );

    pItemV->clearLine();
    for( i=pTableCl->rowCount()-1; i>=0; i-- ){
        pItem = pTableCl->item( i, 0 );
        if( pItem->checkState() == Qt::Checked ){
            pItem = pTableCl->item( i, 2 );
            fQdj = pItem->text().toFloat();

            pItem = pTableCl->item( i, 3 );
            fV = pItem->text().toFloat();

            pItemV->addVLine( fQdj, fV );
        }
    }
    pItemV->drawLine();
}

//�鿴�ٶ���
void QDlgResultQ::slotBrowseVLine( )
{
    drawVLine( );
    pTab->setCurrentIndex( 1 );
}
