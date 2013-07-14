#ifndef QSERIALTHREAD_H
#define QSERIALTHREAD_H

#include <QThread>
#include <QList>

#include "qcommserial.h"
#include "../clscx.h"

#define  STATE_NOTSTART     -1

#define  STATE_HOR          0       //水平移动状态
#define  STATE_HOR_OVER     1       //水平移动状态结束

#define  STATE_VER          2       //垂直向下移动状态
#define  STATE_VER_OVER     3       //向下垂直移动状态结束

#define  STATE_CL           4       //测量状态
#define  STATE_CL_OVER      5       //测量状态结束

#define  STATE_VER_UP       6       //上升过程
#define  STATE_VER_UP_OVER  7       //上升过程结束

#define  CMD_HEAD   0   //出车
#define  CMD_BACK   1   //回车
#define  CMD_UP     2   //上行
#define  CMD_DOWN   3   //下行
#define  CMD_STOP   4   //停止

#define  CMD_CL     5   //测量

#define  MODE_MANU  0   //手动
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
    bool setConfigQdj( QString strValue );
    bool setConfigSs( QString strValue );
    void sendCmdMove( int iCmd );
    void sendCmdCl( );
    void queryMove( );
    void queryCl( );

    void startAutoMode( int iMode );
    void stopAuto( );
    void pauseAuto( );
    void continueAuto( );
    void setHalfContinue( ){ bHalfContinue = true ;}
    void setStateNotStart( ){ iState = STATE_NOTSTART ;}

    void clearCx( ){ listCx.clear( ); }
    void appendCx( clsCx  cx ){ listCx.append( cx ); }
    void printfCx();

    void runManu( );
    void runFullAuto( );

    void runCzff1(  );
    void runCzff2(  );

    void computerLs( );
    void moveFish(  );

    void resetQueryMovingCount( ){ iQueryMovingCount = 0; }

protected:
    void writeComm( char c );
    void readComm( );

private:
    QCommSerial serial ;
    bool bConnected ;
    int iComNo ;
    bool bHalfContinue ;

    unsigned char chBufInput[1024];
    unsigned char chBufOutput[1024];

    int iLenInput ;
    int iLenOutput ;

    float fQdj, fSs ;   //手动时设定的起点距， 水深
    float fPos[7]  ;    //从通讯协议中取来的位置  3 当前起点距 5 当前水深 6 预置起点距 7 预置水深
    float fCl[5]  ;     //流速测量结果  K C T N V
    int iState ;        //状态
    int iQueryMovingCount ; //查询移动的计数

    QList <clsCx>  listCx ;
    int iAutoMode ;     //自动运行模式
    int iAutoState ;    //自动运行状态

    int iCxClid ;       //垂线测量编号
    clsCx  currentCx ;  //当前垂线
//    int iCxClState ;    //垂线测量状态
    bool bSecondHeightIng ; //第二档上提正在进行中

signals:
    void sigFishPos( float * fPos, int iNums, bool bRes );  // bRes 表示是在测量中还是测量结果
    void sigClRes( float * fCl, int iNums, bool bRes );       // false测量过程中   true测量有结果
    void sigSendMsg( QString );
    void sigHalf( );    //半自动阶段完成的信号
    void sigAuto( ) ;   //自动测量完成！

};

#endif // QSERIALTHREAD_H
