#ifndef QCOMMSERIAL_H
#define QCOMMSERIAL_H

#include <QObject>

#include "win_qextserialport.h"

class QCommSerial : public QObject
{
    Q_OBJECT
public:
    QCommSerial( QObject * parent );

    ~QCommSerial( );

    bool openCom( int iComNo, int iBaud, int iDataLen, int iParity, int iStopBit, int iTimeOut );

    QString getMsg( int iComNo, int iBaud, int iDataLen, int iParity, int iStopBit );

    int readCom(unsigned char *chBuf);
    int writeCom( unsigned char * chBuf, int iLen);

private:
    Win_QextSerialPort * myCom;

};

#endif // QCOMMSERIAL_H
