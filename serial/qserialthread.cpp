#include "qserialthread.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QDateTime>

//倍数
static const double fMulti[4][5] = {
    1,0,0,0,0,
    1,1,0,0,0,
    1,1,1,0,0,
    1,3,3,2,1
};

//水深
static const double fRatioSs[4][5] = {
    0.6, 0,   0,   0,    0,
    0.2, 0.8, 0,   0,    0,
    0.2, 0.6, 0.8, 0,    0,
    0,   0.2, 0.6, 0.8,  1.0
};

//一点法和五点法对应的测量点数
//根据测量方法iclff取值
static const double fNums[5] = {
    1,2,3,5
};

QSerialThread::QSerialThread(QObject *parent) :
    QThread( parent ), serial( parent )
{
    iAutoMode = MODE_MANU ;
    iComNo = 6 ;
    iState = STATE_NOTSTART ;       //置测量状态
    iCxClid = 0;                    //置测点位置
    iAutoState = AUTO_STATE_STOP ;  //停止状态
    bHalfContinue = true ;          //半自动测速状态时是否继续
    bSecondHeightIng = false ;      //
}

void QSerialThread::init(  )
{
    bConnected = serial.openCom( iComNo, 2400, 8, 0, 1, 500 );
    *( chBufOutput + 0 ) = 0x50 ;
    qDebug() << bConnected;
    qDebug() << serial.getMsg( iComNo, 2400, 8, 0, 1 );
    if( !bConnected ){
        QMessageBox::warning(NULL, tr(""), tr("串口打开失败! "));
    }
}

void QSerialThread::run( )
{
    if( !bConnected )  return ;

    while( true ){
        switch( iAutoMode  ){
        case MODE_MANU:
            runManu( );
            break ;
        case MODE_HALF:
        case MODE_AUTO:
            runFullAuto( );
            break;
        default:
            break;
        }
    }
}

//手动执行的处理
void QSerialThread::runManu( )
{
    switch( iState ){
    case STATE_HOR:
    case STATE_VER:
        queryMove( );
        break;
    case STATE_HOR_OVER:
    case STATE_VER_OVER:
    case STATE_VER_UP_OVER:
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

//全自动处理的执行
void QSerialThread::runFullAuto( )
{
    if( iAutoState == AUTO_STATE_STOP ) {
        msleep(200);
        return ;
    }
    bool b1 ;
    b1 = (iState ==  STATE_VER_OVER || iState ==STATE_HOR_OVER || STATE_VER_UP_OVER );

    if( b1 && iAutoState == AUTO_STATE_PAUSE ){
        msleep(200);
        return ;
    }

//    qDebug( ) << 0  << bHalfContinue ;
    if( iAutoMode == MODE_HALF && !bHalfContinue ){ //半自动模式下继续标志
        msleep(200);
        return ;
    }
//    qDebug( ) << 1 ;
    switch( iState ){
    case STATE_NOTSTART:
        moveFish(  );
        break ;

    case STATE_VER_UP:
    case STATE_HOR:
    case STATE_VER:
        queryMove( );
        break;
    case STATE_HOR_OVER:
    {
        currentCx.fCdSs[iCxClid] = currentCx.fSs * fRatioSs[currentCx.iClff][iCxClid];
//        qDebug() << tr("水深") << currentCx.fCdSs[iCxClid] ;

        QString strSs = QString("%1").arg(currentCx.fSs * fRatioSs[currentCx.iClff][iCxClid]);
        setConfigSs( strSs );    //置水深
        sendCmdMove( CMD_DOWN );    //铅鱼下送
    }
        break;
    case STATE_VER_OVER:
        sendCmdCl( );               //开始测量
        break;
    case STATE_VER_UP_OVER:
        moveFish(  );
        break;
    case STATE_CL:
        queryCl( );
        break;
    case STATE_CL_OVER:
        iCxClid ++ ;    //找下一个测点
        if( iCxClid >= fNums[currentCx.iClff] ){
            computerLs( );
            sendCmdMove( CMD_UP );
            if( iAutoMode == MODE_HALF && listCx.size()>0 ){   //半自动发消息，置停止标志
                emit sigHalf( );
                bHalfContinue = false ;
            }
        }
        else{
            currentCx.fCdSs[iCxClid] = currentCx.fSs * fRatioSs[currentCx.iClff][iCxClid];
            QString strSs = QString("%1").arg(currentCx.fSs * fRatioSs[currentCx.iClff][iCxClid]);
            setConfigSs( strSs );    //置水深
            sendCmdMove( CMD_DOWN );    //铅鱼下送
        }
        break;
    default:
        msleep(200);
        break;
    }
}

//操作方法1  应用水深
void QSerialThread::runCzff1(  )
{

}

//操作方法2  先测水深
void QSerialThread::runCzff2(  )
{

}

//计算流速
void QSerialThread::computerLs( )
{
    float fLs ;
    int i;
    float sum = 0 , sum1 = 0 ;
    for(i=0; i<fNums[currentCx.iClff]; i++){
        sum += (fMulti[currentCx.iClff][i] * currentCx.fV[i]);
        sum1 += fMulti[currentCx.iClff][i] ;
    }
    fLs = sum/sum1 ;

    QString strV, strT, strN, strK, strC, strCdSs ;
    int iLen ;

    strK = QString("%1").arg( currentCx.fK );
    strC = QString("%1").arg( currentCx.fC );

    for(i=0; i<fNums[currentCx.iClff]; i++){
        strV += QString("%1/").arg(currentCx.fV[i]);
    }
    iLen = strV.length() ;
    strV = strV.left( iLen - 1 );

    for(i=0; i<fNums[currentCx.iClff]; i++){
        strT += QString("%1/").arg(currentCx.fT[i]);
    }
    iLen = strT.length();
    strT = strT.left( iLen - 1 );

    for(i=0; i<fNums[currentCx.iClff]; i++){
        strN += QString("%1/").arg(currentCx.fN[i]);
    }
    iLen = strN.length() ;
    strN = strN.left( iLen - 1 );

    for(i=0; i<fNums[currentCx.iClff]; i++){
        strCdSs += QString("%1/").arg(currentCx.fCdSs[i]);
    }
    iLen = strCdSs.length() ;
    strCdSs = strCdSs.left( iLen - 1 );

    QString strSql ;

    QDateTime dt = QDateTime::currentDateTime() ;
    QDate d = dt.date() ;
    QTime t = dt.time() ;

    QString strTime ;
    strTime.sprintf( "%04d-%02d-%02d %02d:%02d:%02d ", d.year(),d.month(),d.day(),t.hour(),t.minute(),t.second() );

    strSql = QString("insert into result( dt, qdj, ss, ls, fV, fT, fN, fK, fC, fCdSs ) values( '%1', %2, %3, %4, '%5', '%6', '%7', '%8', '%9', '%10' )" )
            .arg(strTime).arg( currentCx.fQdj ).arg( currentCx.fSs ).arg( fLs )
            .arg(strV).arg(strT).arg(strN).arg(strK).arg(strC)
            .arg(strCdSs);

    QSqlQuery query;
    query.exec( strSql );

}

void QSerialThread::moveFish(  )
{
    iCxClid = 0;
    if( listCx.isEmpty() ){
        if( bSecondHeightIng ){     //第二次提升完成后结束全自动过程
            iAutoState = AUTO_STATE_STOP ;
            iState = STATE_NOTSTART ;
            iAutoMode = MODE_MANU ;
            bSecondHeightIng = false ;      //
            emit sigAuto( );
        }
        else{
            bSecondHeightIng = true ;      //
            sendCmdMove( CMD_UP );
        }

        return ;
    }
    else{
        currentCx = listCx.front();
        listCx.pop_front();
    }
    setConfigQdj( QString("%1").arg( currentCx.fQdj ) );

    //根据当前位置判断是出车还是回车，先查询位置
    bool bRes ;
    bRes = queryMove( );   //查询铅鱼位置
    if( !bRes )
        bRes = queryMove( );   //查询铅鱼位置
    if( !bRes )  return ;

    qDebug( ) << "move fish pos justify" << *(fPos + 3) << currentCx.fQdj  ;
    if( *(fPos + 3) < currentCx.fQdj )
        sendCmdMove( CMD_HEAD );
    else
        sendCmdMove( CMD_BACK );
}

void QSerialThread::writeComm( char c )
{
    * chBufOutput = c ;
//    qDebug( ) << * chBufOutput ;
    int iLen ;
    iLen = serial.writeCom( chBufOutput, 1 );    //写串口k
    qDebug( ) << "send " << QString( c )  ;
}

void QSerialThread::readComm( )
{
    iLenInput = serial.readCom( chBufInput );
    *(chBufInput + iLenInput ) = 0 ;
    qDebug( ) << "get " << QString( (char*)chBufInput );
}

//查询铅鱼移动
static char ay[] = "AY" ;   //还在行走过程中
static char py[] = "PY" ;   //行走结束
//00 0 00 +010.0  -01.07   010.0  000.0AY
//00 0 00 +010.0  -01.07   010.0  000.0PY

//bool bVerifyTime  是否验证时间的标志
bool QSerialThread::queryMove(  )
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

    if( !(bResult2 || bResult1) )  return false ;

//    qDebug( ) << "result" << bResult1 << bResult2 ;

    bool bRes = false ;
    if( bResult1 ) { iQueryMovingCount++; msleep(500); bRes = false ; }

    if( bResult2 ) {
        if( iState == STATE_HOR && iQueryMovingCount > 0 )
            iState = STATE_HOR_OVER ;
        if( iState == STATE_VER && iQueryMovingCount > 0 )
            iState = STATE_VER_OVER ;
        if( iState == STATE_VER_UP && iQueryMovingCount > 0 )
            iState = STATE_VER_UP_OVER ;
        bRes = true ;
    }

    *( chBufInput + iLenInput - 3 ) = 0 ;
    qDebug( ) << (char*)chBufInput  ;
//    qDebug( ) << *chBufInput << *(chBufInput+1)<< *(chBufInput+2) ;

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
        emit sigSendMsg(tr("铅鱼到位停止!") );

    return true ;

}

bool QSerialThread::setConfigQdj( QString strValue )
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
    serial.writeCom( (unsigned char*)chValue, 4 );    //写串口k
    msleep(500);
    readComm( );

    emit sigSendMsg(tr("设置起点距完成!") + QString("%1").arg( (char*)chBufInput  )  );

    //判断设备是否成功，如果成功执行下一步，如果不成功则重试一次
    QString strResult ;
    bool bRes = false ;
    strResult.sprintf("%s",  (char*)chBufInput );
    bRes = strResult.contains( QString("%1").arg(chValue)  );

    if( bRes ) return true ;

    //第二次设置起点距参数
    serial.writeCom( (unsigned char*)chValue, 4 );    //写串口k
    msleep(500);
    readComm( );
    emit sigSendMsg(tr("设置起点距完成!") + QString("%1").arg( (char*)chBufInput  )  );
    strResult.sprintf("%s",  (char*)chBufInput );
    bRes = strResult.contains( QString("%1").arg(chValue)  );

    return bRes ;

}

bool QSerialThread::setConfigSs( QString strValue )
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
    serial.writeCom( (unsigned char*)chValue, 4 );    //写串口k
    msleep(500);
    readComm( );

    emit sigSendMsg(tr("设置水深完成!") + QString("%1").arg( (char*)chBufInput ) );

    //判断设备是否成功，如果成功执行下一步，如果不成功则重试一次
    QString strResult ;
    bool bRes = false ;
    strResult.sprintf("%s",  (char*)chBufInput );
    bRes = strResult.contains( QString("%1").arg(chValue)  );

    if( bRes ) return true ;

    //第二次设置水深参数
    serial.writeCom( (unsigned char*)chValue, 4 );    //写串口k
    msleep(500);
    readComm( );
    emit sigSendMsg(tr("设置水深完成!") + QString("%1").arg( (char*)chBufInput  )  );
    strResult.sprintf("%s",  (char*)chBufInput );
    bRes = strResult.contains( QString("%1").arg(chValue)  );

    return bRes ;
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
        iState = STATE_VER_UP ;
        break;
    case CMD_DOWN:
        iState = STATE_VER ;
        break;
    case CMD_STOP:
        if( iAutoMode == MODE_MANU )
            iState = STATE_NOTSTART ;
        break;
    default:
        break;
    }

    iQueryMovingCount = 0; //计数清零
}

static char chCL[3][2] = { 'l', 'b', 'l', 'r', 'l', 'a' };
void QSerialThread::sendCmdCl(  )
{
    writeComm( chCL[0][0] );
    msleep(500);
    readComm( );

    writeComm( chCL[0][1] );
    msleep(500);
    readComm( );


    writeComm( chCL[1][0] );
    msleep(500);
    readComm( );

    writeComm( chCL[1][1] );
    msleep(500);
    readComm( );

    iState = STATE_CL ; //置测量状态
}

//查询测量过程
//查询铅鱼移动
static char yw[] = "YW" ;   //还在测量过程中
static char zw[] = "ZW" ;   //测量结束
// aK0.2500 C0.0111 T000.4 N0000 V0.000YW
// aT000.8 N0000YW
// aK0.2500 C0.0111 T011.4 N0100 V2.204ZW
bool QSerialThread::queryCl( )
{
    writeComm( chCL[2][0] );
    msleep(500);
    readComm( );

    writeComm( chCL[2][1] );
    msleep(1500);
    readComm( );

    bool bResult1, bResult2  ;
    bResult1 = ( memcmp( chBufInput + iLenInput - 3, yw , 2 ) == 0 );
    bResult2 = ( memcmp( chBufInput + iLenInput - 3, zw , 2 ) == 0 );
    if( !(bResult2 || bResult1) )  return false ;
    qDebug( ) << (char*)chBufInput  ;
//    qDebug( ) << "result" << bResult1 << bResult2 ;

    bool bRes = false ;
    if( bResult1 ) { msleep(500);  bRes = false ;}

    if( bResult2 ) {
        iState = STATE_CL_OVER ;
        bRes = true ;
    }

    *( chBufInput + iLenInput - 3 ) = 0;

//    qDebug( ) << "trim" << (char*)chBufInput  ;

    char chMark[5] ;
    float f[2];
    int iRes ;
    iRes = sscanf( (char*)chBufInput, "%3c%f %c%f %c%f %c%f %c%f", chMark, f, \
           chMark, f+1, chMark, fCl+2, chMark, fCl+3, \
                  chMark, fCl+4 );

//    qDebug() << "float " << f[0] << "|" << f[1] ;

    qDebug() << "result" << iRes ;
    if( iRes == 10 ){   //第一次和测量结果的采数结果
        fCl[0] = f[0];
        fCl[1] = f[1];
//        qDebug( ) << "result  1_" << f[0] << f[1] ;
    }

    if( iRes == 4 ){    //第2到N次的采数结果
        fCl[2] = f[0];
        fCl[3] = f[1];
        qDebug( ) << "result 2_" << f[0]  << f[1] ;
    }

    if( iRes != 4 && iRes != 10 )  return  false ;

//    for(int i=0; i<5; i++ ){
//        qDebug( ) << i<< *(fCl+i) ;
//    }

    if( bResult1 ){
        *( fCl + 4 ) = 0;
        emit sigClRes( fCl, 5, bRes );
    }

    if( bResult2 ) {
        //发 lb 返回到初始状态
        msleep(500);

        writeComm( chCL[0][0] );
        msleep(500);
        readComm( );

        writeComm( chCL[0][1] );
        msleep(500);
        readComm( );

        writeComm( chCL[1][0] );
        msleep(500);
        readComm( );

        writeComm( chCL[1][1] );
        msleep(500);
        readComm( );

        //发结果信号
        emit sigClRes( fCl, 5, bRes );
        emit sigSendMsg(tr("测量流速完成!") );


        //存储测量结果  手动测量
        if( iAutoMode == MODE_MANU ){
            QString strSql ;

            QDateTime dt = QDateTime::currentDateTime() ;
            QDate d = dt.date() ;
            QTime t = dt.time() ;

            QString strTime ;
            strTime.sprintf( "%04d-%02d-%02d %02d:%02d:%02d ", d.year(),d.month(),d.day(),t.hour(),t.minute(),t.second() );

            strSql = QString("insert into result( dt, qdj, ss, ls, fV, fT, fN, fK, fC, fCdSs ) values( '%1', %2, %3, %4, '%5', '%6', '%7', '%8', '%9', '%10' )" )
                    .arg(strTime).arg(fQdj).arg(fSs).arg( *( fCl+4 ) )
                    .arg(QString("%1").arg( *(fCl+4) )) //V
                    .arg(QString("%1").arg( *(fCl+2) )) //T
                    .arg(QString("%1").arg( *(fCl+3) )) //N
                    .arg(QString("%1").arg( *(fCl+0) )) //K
                    .arg(QString("%1").arg( *(fCl+1) )) //C
                    .arg( QString("%1").arg( fSs ) );

            QSqlQuery query;
            query.exec( strSql );
        }

        if( iAutoMode == MODE_HALF || iAutoMode == MODE_AUTO ){
            currentCx.fK = *( fCl+0 ) ;
            currentCx.fC = *( fCl+1 ) ;
            currentCx.fT[iCxClid] = *( fCl+2 ) ;
            currentCx.fV[iCxClid] = *( fCl+4 ) ;
            currentCx.fN[iCxClid] = *( fCl+3 ) ;
        }
    }
    return true ;
}

//开启自模式
void QSerialThread::startAutoMode( int iMode )
{
    iAutoMode = iMode ;
    iAutoState = AUTO_STATE_START ;
    bHalfContinue = true ;
}


//停止自动测速模式
void QSerialThread::stopAuto( )
{
    listCx.clear();
    iAutoState = AUTO_STATE_STOP ;
    iState = STATE_NOTSTART ;
    bSecondHeightIng = false ;      //
    iAutoMode = MODE_MANU ;
}


//暂停自动测速模式
void QSerialThread::pauseAuto( )
{
    iAutoState = AUTO_STATE_PAUSE ;
}

//继续自动测速
void QSerialThread::continueAuto( )
{
    iAutoState = AUTO_STATE_START ;
}

//打印
void QSerialThread::printfCx( )
{
    for(int i=0; i<listCx.size(); i++ ){
        listCx[i].printf( );
    }
}
