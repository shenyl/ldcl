#include "qcommserial.h"

QCommSerial::QCommSerial( QObject *parent )
    :QObject( parent )
{
    myCom = NULL ;
}

QCommSerial::~QCommSerial()
{
    if( myCom != NULL ){
        myCom->close();
        delete myCom;
    }
}

//�򿪴��ڹ���
//int iComNo    ���ں�
//int iBaud     ������  9600
//int iDataLen  ����λ���� 7  8
//int iParity   0 �� 1 �� 2 ż 3 mark
//int iStopBit  ֹͣλ  1 2  15  15 ��1.5
//int iTimeOut  ��ʱ

bool QCommSerial::openCom( int iComNo, int iBaud, int iDataLen, int iParity, int iStopBit, int iTimeOut )
{
    QString strComNo = QString("\\\\.\\COM%1").arg( iComNo );

    myCom = new Win_QextSerialPort( strComNo ,QextSerialBase::EventDriven );

    bool bConnected ;
    bConnected  = myCom->open(QIODevice::ReadWrite);

    //���ò�����
    BaudRateType baud ;
    switch( iBaud ){
    case 9600:
        baud = BAUD9600 ;
        break ;
    case 115200:
        baud = BAUD115200 ;
        break ;
    case 2400:
        baud = BAUD2400 ;
        break ;
    case 300:
        baud = BAUD300 ;
        break ;
    case 600:
        baud = BAUD600 ;
        break ;
    case 19200:
        baud = BAUD19200 ;
        break ;
    default:
        break ;
    }

    myCom->setBaudRate( baud );

    if( iDataLen == 8 )  myCom->setDataBits(DATA_8);
    if( iDataLen == 7 )  myCom->setDataBits(DATA_7);

    if( iParity == 0 )  myCom->setParity(PAR_NONE);
    if( iParity == 1 )  myCom->setParity(PAR_ODD);
    if( iParity == 2 )  myCom->setParity(PAR_EVEN);
    if( iParity == 3 )  myCom->setParity(PAR_MARK);

    if( iStopBit == 1 )  myCom->setStopBits(STOP_1);
    if( iStopBit == 2 )  myCom->setStopBits(STOP_2);
    if( iStopBit == 15 )  myCom->setStopBits(STOP_1_5);

    myCom->setFlowControl( FLOW_OFF );
    myCom->setTimeout( iTimeOut ); //������ʱ
    return  bConnected ;
}

//�������� com1 8-n-1 ������
QString QCommSerial::getMsg( int iComNo, int iBaud, int iDataLen, int iParity, int iStopBit )
{
    QString strCom = QString( "com%1 %2-" ).arg( iComNo ).arg( iDataLen ) ;

    QString strParity ;
    if( iParity == 0 )  strParity = "N" ;
    if( iParity == 1 )  strParity = "O" ;
    if( iParity == 2 )  strParity = "E" ;
    if( iParity == 3 )  strParity = "M" ;

    strCom.append( strParity );
    strCom.append( "-" );
    if( iStopBit == 1 || iStopBit == 2 )
        strCom.append( QString("%1").arg( iStopBit ) );
    if( iStopBit == 15 )
        strCom.append( "1.5 " );

    strCom.append( QString(" %1").arg(iBaud) );

    return strCom ;
}

int QCommSerial::readCom(unsigned char *chBuf)
{
   return myCom->read((char*)chBuf, 1024);
}

int QCommSerial::writeCom( unsigned char * chBuf, int iLen )
{
    return myCom->write((char*)chBuf, iLen);;
}
