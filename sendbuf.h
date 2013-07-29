#ifndef SENDBUF_H
#define SENDBUF_H

#include <QObject>
#include <QList>
#include <QThread>

#define  KIND_WHOLE     1
#define  KIND_CHAR      2

class  clsBuf
{
public:
    clsBuf( ){
        iLen = 0;
        iKind = 0;
    }
    unsigned char chBuf[512];
    int iLen ;
    int iKind ;     //1 是整串发出  2 分字符发出
};

class QCommSerial;

class QThreadSleep : public QThread
{
    Q_OBJECT

public:
    explicit QThreadSleep(QObject *parent = 0): QThread( parent ) {  }
    void sleep( int iTime ){ msleep( iTime ); }
};

class QSendBuf : public QObject
{
    Q_OBJECT
public:
    explicit QSendBuf(QObject *parent = 0);

    void addBuf( unsigned char * chBuf, int iLen, int iKind );

    void clear( ){ listBuf.clear(); }
    int getCount( ){ return listBuf.count(); }
    void setGo( ){ bGo = true; }

    void setMem( QCommSerial * pSerial );

    QList<clsBuf> listBuf ;
    QCommSerial * pSerial ;
    unsigned char chBufInput[512];
    int iCount ;
    QThreadSleep st ;
    bool bGo ;

protected:
    void timerEvent ( QTimerEvent * event );
    bool sendKindWhole( clsBuf  b );
    bool sendKindChar( clsBuf b );
signals:
    void sigMsg( QString );

public slots:

};

#endif //SENDBUF_H
