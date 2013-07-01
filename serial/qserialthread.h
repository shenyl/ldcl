#ifndef QSERIALTHREAD_H
#define QSERIALTHREAD_H

#include <QThread>
#include <QList>

#include "qcommserial.h"
#include "../clscx.h"

#define  STATE_HOR          0       //水平移动状态
#define  STATE_HOR_OVER     1       //水平移动状态结束

#define  STATE_VER          2       //垂直移动状态
#define  STATE_VER_OVER     3       //垂直移动状态结束

#define  STATE_CL           4       //测量状态
#define  STATE_CL_OVER      5       //测量状态结束

#define  CMD_HEAD   0   //出车
#define  CMD_BACK   1   //回车
#define  CMD_UP     2   //上行
#define  CMD_DOWN   3   //下行
#define  CMD_STOP   4   //停止

#define  CMD_CL     5   //测量

#define  MODE_HALF  1   //半自动
#define  MODE_AUTO  2   //全自动

#define  AUTO_STATE_START  1
#define  AUTO_STATE_PAUSE  2
#define  AUTO_STATE_STOP   3

class QSerialThread : public QThread
{
    Q_OBJECT

public:
    explicit QSerialThread(QObject *parent = 0);

    void init( );
    void run( );

    void setComNo( int iComNo ){ this->iComNo = iComNo; }
    void setConfigQdj( QString strValue );
    void setConfigSs( QString strValue );
    void sendCmdMove( int iCmd );
    void sendCmdCl( );
    void queryMove( );
    void queryCl( );

    void startAutoMode( int iMode );
    void stopAuto( );
    void pauseAuto( );

    void appendCx( clsCx  cx ){ listCx.append( cx ); }
    void printfCx();

protected:
    void parseInput(  );

    void writeComm( char c );
    void readComm( );


private:
    QCommSerial serial ;
    bool bConnected ;
    int iComNo ;

    unsigned char chBufInput[1024];
    unsigned char chBufOutput[1024];

    int iLenInput ;
    int iLenOutput ;

    float fQdj, fSs ;   //起点距， 水深
    float fPos[7]  ;    //从通讯协议中取来的位置  4 当前起点距 5 当前水深 6 预置起点距 7 预置水深
    float fCl[5]  ;     //流速测量结果  K C D N V
    int iState ;        //状态

    QList <clsCx>  listCx ;
    int iAutoMode ;
    int iAutoState ;

signals:

    void sigFishPos( float * fPos, int iNums, bool bRes );  // bRes 表示是在测量中还是测量结果
    void sigClRes( float * fCl, int iNums, bool bRes );       // false测量过程中   true测量有结果
    void sigSendMsg( QString );

};

#endif // QSERIALTHREAD_H
