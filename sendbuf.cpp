#include "sendbuf.h"

#include <QDebug>

#include "./serial/qcommserial.h"

QSendBuf::QSendBuf(QObject *parent) :
    QObject(parent)
{

}

void QSendBuf::addBuf( unsigned char * chBuf, int iLen, int iKind )
{
    clsBuf buf ;
    memcpy( buf.chBuf, chBuf, iLen );
    buf.iLen = iLen ;
    buf.iKind = iKind ;

    listBuf.append( buf );
    bGo = false ;
}

void QSendBuf::setMem( QCommSerial * pSerial )
{
    this->pSerial = pSerial ;
    startTimer( 500 );
    iCount = 0 ;
}

void QSendBuf::timerEvent( QTimerEvent * event )
{
//    qDebug( ) << "buf";
    if( !bGo ) return ;

    if( listBuf.size() == 0 ) return ;
    if( iCount > 0 ) iCount -- ;

    bool bRes ;
    for(int i=0; i< listBuf.size(); i++ ){
        bRes = false ;
        if( listBuf.at(i).iKind == KIND_WHOLE )
            bRes = sendKindWhole( listBuf.at(i) );
        if( listBuf.at(i).iKind == KIND_CHAR )
            bRes = sendKindChar( listBuf.at(i) );
        if( !bRes ){
            iCount = 5;
            return ;
        }
    }
    listBuf.clear();
}

//整串发送
//特征是回串有整串发送的内容
bool QSendBuf::sendKindWhole( clsBuf b )
{
    int iLen ;
    iLen = pSerial->writeCom( b.chBuf, b.iLen );
    *( b.chBuf + b.iLen ) = 0 ;
    qDebug( ) << "write comm: " << QString("%1").arg( (char *)b.chBuf );
    emit sigMsg( QString("send: %1").arg( QString("%1").arg( (char *)b.chBuf )));

    st.sleep( 500 );
    iLen = pSerial->readCom( chBufInput );
    *(chBufInput + iLen ) = 0 ;
    qDebug( ) << "read comm: " << QString("%1").arg( (char *)chBufInput );

    emit sigMsg( QString("get: %1").arg( (char *)chBufInput ) );

    QString strResult ;
    bool bRes = false ;
    strResult.sprintf("%s",  (char*)chBufInput );

    bRes = strResult.contains( QString("%1").arg(( char * )b.chBuf) );
    return bRes ;
}

//单字符发送
bool QSendBuf::sendKindChar( clsBuf b )
{
    int i, iLen ;
    QString strWrite ;
    for( i=0; i<b.iLen; i++ ){
        iLen = pSerial->writeCom( b.chBuf+i, 1 );
        strWrite.sprintf("%c", *(b.chBuf+i) );
        qDebug( ) << "write comm: " << strWrite << "bin" << *(b.chBuf+i);
        st.sleep( 500 );
        iLen = pSerial->readCom( chBufInput );
        *( chBufInput + iLen ) = 0 ;
        qDebug( ) << "read comm  length " << iLen ;
        qDebug( ) << "read comm str " << QString("%1").arg( (char*)chBufInput ) ;
        for( int j=0; j<iLen; j++ ){
            qDebug( ) << "bin " << QString("%1").arg( *(chBufInput+j) );
        }
        if( *chBufInput != *(b.chBuf+i) ) return false;
    }

    return true;
}

//    qDebug( ) << * listBuf.at(0).chBuf << *(listBuf.at(0).chBuf +1)
//        <<*( listBuf.at(0).chBuf + 2 );
