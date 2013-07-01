#include "qserialthread.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>

QSerialThread::QSerialThread(QObject *parent) :
    QThread( parent ), serial( parent )
{
    iComNo = 6 ;
    iState = -1 ;
}

void QSerialThread::init(  )
{
    bConnected = serial.openCom( iComNo, 2400, 8, 0, 1, 500 );
    *( chBufOutput + 0 ) = 0x50 ;
    qDebug() << bConnected;
    qDebug() << serial.getMsg( iComNo, 2400, 8, 0, 1 );
    if( !bConnected ){
        QMessageBox::warning(NULL, tr(""), tr("���ڴ�ʧ��! "));
    }
}

void QSerialThread::run( )
{
    while( bConnected ){
        switch( iState ){
        case STATE_HOR:
        case STATE_VER:
            queryMove( );
            break;
        case STATE_HOR_OVER:
        case STATE_VER_OVER:
            msleep(200);
            break;
        case STATE_CL:
            queryCl( );
            break;
        case STATE_CL_OVER:
            break;
        default:
            msleep(200);
            break;
        }
    }
}

//���������ͨѶ��
void QSerialThread::parseInput(  )
{

}

void QSerialThread::writeComm( char c )
{
    * chBufOutput = c ;
//    qDebug( ) << * chBufOutput ;
    int iLen ;
    iLen = serial.writeCom( chBufOutput, 1 );    //д����k
    qDebug( ) << "send " << QString( c )  ;
}

void QSerialThread::readComm( )
{
    iLenInput = serial.readCom( chBufInput );
    *(chBufInput + iLenInput ) = 0 ;
    qDebug( ) << "get " << QString( (char*)chBufInput );
}

//��ѯǦ���ƶ�
static char ay[] = "AY" ;   //�������߹�����
static char py[] = "PY" ;   //���߽���
//00 0 00 +010.0  -01.07   010.0  000.0AY
//00 0 00 +010.0  -01.07   010.0  000.0PY
void QSerialThread::queryMove( )
{
    writeComm( 'j' );
    msleep(500);
    readComm( );

    msleep(500);

    writeComm( 'd' );
    msleep(1000);
    readComm( );

    bool bResult1, bResult2 ;
    bResult1 = ( memcmp( chBufInput + iLenInput - 3, ay , 2 ) == 0 );
    bResult2 = ( memcmp( chBufInput + iLenInput - 3, py , 2 ) == 0 );

//    qDebug( ) << "result" << bResult1 << bResult2 ;

    bool bRes = false ;
    if( bResult1 ) { msleep(500); bRes = false ; }

    if( bResult2 ) {
        if( iState == STATE_HOR )
            iState = STATE_HOR_OVER ;
        if( iState == STATE_VER )
            iState = STATE_VER_OVER ;
        bRes = true ;
    }

    *( chBufInput + iLenInput - 3 ) = 0 ;
//    qDebug( ) << (char*)chBufInput  ;

    int iRes ;
    char s[50];
    iRes = sscanf( (char*)chBufInput, "%c%f %f %f %f %f %f %f",  s, fPos, \
                   fPos + 1, fPos + 2, fPos + 3, fPos + 4, fPos + 5, \
                   fPos + 6 );

    //    for(int i=0; i<7; i++ ){
    //        qDebug( ) << i<< *(fPos+i) ;
    //    }

    emit sigFishPos( fPos, 7, bRes );
    if(bResult2)
        emit sigSendMsg(tr("Ǧ�㵽λֹͣ!") );

}

void QSerialThread::setConfigQdj( QString strValue )
{
    writeComm( 'j' );
    msleep(500);
    readComm( );

    writeComm( 'g' );
    msleep(500);
    readComm( );

    fQdj = strValue.toFloat( );
    float f =  fQdj * 10 ;
    int iValue = (int)f ;
    char chValue[10];
    sprintf( chValue,"%04d", iValue );
    serial.writeCom( (unsigned char*)chValue, 4 );    //д����k
    msleep(500);
    readComm( );

    emit sigSendMsg(tr("�����������!") + QString("%1").arg( (char*)chBufInput  )  );
}

void QSerialThread::setConfigSs( QString strValue )
{
    writeComm( 'j' );
    msleep(500);
    readComm( );

    writeComm( 'i' );
    msleep(500);
    readComm( );

    fSs = strValue.toFloat();
    float f =  fSs * 100 ;
    int iValue = (int)f ;
    char chValue[10];
    sprintf( chValue,"%04d", iValue );
    serial.writeCom( (unsigned char*)chValue, 4 );    //д����k
    msleep(500);
    readComm( );

    emit sigSendMsg(tr("����ˮ�����!") + QString("%1").arg( (char*)chBufInput ) );
}

static char chCmd[5][2] = { 'l', 'w', 'l', 'n', 'l', 'y', 'l', 'x', 'l', 'm' };

void QSerialThread::sendCmdMove( int iCmd )
{
    writeComm( chCmd[iCmd][0] );
    msleep(500);
    readComm( );

    writeComm( chCmd[iCmd][1] );
    msleep(500);
    readComm( );

    switch(iCmd){
    case CMD_HEAD:
    case CMD_BACK:
        iState = STATE_HOR ;
        break;
    case CMD_UP:
    case CMD_DOWN:
        iState = STATE_VER ;
        break;
    default:
        break;
    }

}

static char chCL[3][2] = { 'l', 'b', 'l', 'r', 'l', 'a' };
void QSerialThread::sendCmdCl(  )
{
    writeComm( chCL[0][0] );
    msleep(500);
    readComm( );

    writeComm( chCL[0][1] );
    msleep(2500);
    readComm( );


    writeComm( chCL[1][0] );
    msleep(1000);
    readComm( );

    writeComm( chCL[1][1] );
    msleep(1000);
    readComm( );

    iState = STATE_CL ; //�ò���״̬
}

//��ѯ��������
//��ѯǦ���ƶ�
static char yw[] = "YW" ;   //���ڲ���������
static char zw[] = "ZW" ;   //��������
// aK0.2500 C0.0111 T000.4 N0000 V0.000YW
// aT000.8 N0000YW
// aK0.2500 C0.0111 T011.4 N0100 V2.204ZW
void QSerialThread::queryCl( )
{
    writeComm( chCL[2][0] );
    msleep(500);
    readComm( );

    writeComm( chCL[2][1] );
    msleep(1500);
    readComm( );

    bool bResult1, bResult2 ;
    bResult1 = ( memcmp( chBufInput + iLenInput - 3, yw , 2 ) == 0 );
    bResult2 = ( memcmp( chBufInput + iLenInput - 3, zw , 2 ) == 0 );
    qDebug( ) << (char*)chBufInput  ;
    qDebug( ) << "result" << bResult1 << bResult2 ;

    bool bRes = false ;
    if( bResult1 ) { msleep(500);  bRes = false ;}

    if( bResult2 ) {
        iState = STATE_CL_OVER ;
        bRes = true ;
    }

    *( chBufInput + iLenInput - 3 ) = 0 ;

//    qDebug( ) << "trim" << (char*)chBufInput  ;

    char chMark[5] ;
    float f[2];
    int iRes ;
    iRes = sscanf( (char*)chBufInput, "%3c%f %c%f %c%f %c%f %c%f", chMark, f, \
           chMark, f+1, chMark, fCl+2, chMark, fCl+3, \
                  chMark, fCl+4 );

    qDebug() << "float " << f[0] << "|" << f[1] ;

    qDebug() << "result" << iRes ;

    if( iRes == 10 ){   //��һ�κͲ�������Ĳ������
        fCl[0] = f[0];
        fCl[1] = f[1];
    }

    if( iRes == 4 ){    //��2��N�εĲ������
        fCl[2] = f[0];
        fCl[3] = f[1];
    }

    for(int i=0; i<5; i++ ){
        qDebug( ) << i<< *(fCl+i) ;
    }

    if( bResult1 ){
        *( fCl + 4 ) = 0;
        emit sigClRes( fCl, 5, bRes );
    }

    if( bResult2 ) {
        //�� lb ���ص���ʼ״̬
        msleep(500);

        writeComm( chCL[0][0] );
        msleep(500);
        readComm( );

        writeComm( chCL[0][1] );
        msleep(2500);
        readComm( );

        writeComm( chCL[1][0] );
        msleep(500);
        readComm( );

        writeComm( chCL[1][1] );
        msleep(2500);
        readComm( );

        //������ź�
        emit sigClRes( fCl, 5, bRes );
        emit sigSendMsg(tr("�����������!") );

        //�洢�������
        QString strSql ;

        QDateTime dt = QDateTime::currentDateTime() ;
        QDate d = dt.date() ;
        QTime t = dt.time() ;

        QString strTime ;
        strTime.sprintf( "%04d-%02d-%02d %02d:%02d:%02d ", d.year(),d.month(),d.day(),t.hour(),t.minute(),t.second() );

        strSql = QString("insert into result( dt, qdj, ss, ls ) values( '%1', %2, %3, %4 )" )
                .arg(strTime).arg(fQdj).arg(fSs).arg( *( fCl+4 ) );

        QSqlQuery query;
        query.exec( strSql );
    }
}

//������ģʽ
void QSerialThread::startAutoMode( int iMode )
{
    iAutoMode = iMode ;
    iAutoState = AUTO_STATE_START ;
}

//ֹͣ�Զ�����ģʽ
void QSerialThread::stopAuto( )
{
    listCx.clear();
    iAutoState = AUTO_STATE_STOP ;
}

//��ͣ�Զ�����ģʽ
void QSerialThread::pauseAuto( )
{
    iAutoState = AUTO_STATE_PAUSE ;
}

//��ӡ
void QSerialThread::printfCx( )
{
    for(int i=0; i<listCx.size(); i++ ){
        listCx[i].printf( );
    }
}
