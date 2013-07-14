#ifndef QSERIALTHREAD_H
#define QSERIALTHREAD_H

#include <QThread>
#include <QList>

#include "qcommserial.h"
#include "../clscx.h"

#define  STATE_NOTSTART     -1

#define  STATE_HOR          0       //ˮƽ�ƶ�״̬
#define  STATE_HOR_OVER     1       //ˮƽ�ƶ�״̬����

#define  STATE_VER          2       //��ֱ�����ƶ�״̬
#define  STATE_VER_OVER     3       //���´�ֱ�ƶ�״̬����

#define  STATE_CL           4       //����״̬
#define  STATE_CL_OVER      5       //����״̬����

#define  STATE_VER_UP       6       //��������
#define  STATE_VER_UP_OVER  7       //�������̽���

#define  CMD_HEAD   0   //����
#define  CMD_BACK   1   //�س�
#define  CMD_UP     2   //����
#define  CMD_DOWN   3   //����
#define  CMD_STOP   4   //ֹͣ

#define  CMD_CL     5   //����

#define  MODE_MANU  0   //�ֶ�
#define  MODE_HALF  1   //���Զ�
#define  MODE_AUTO  2   //ȫ�Զ�

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

    float fQdj, fSs ;   //�ֶ�ʱ�趨�����࣬ ˮ��
    float fPos[7]  ;    //��ͨѶЭ����ȡ����λ��  3 ��ǰ���� 5 ��ǰˮ�� 6 Ԥ������ 7 Ԥ��ˮ��
    float fCl[5]  ;     //���ٲ������  K C T N V
    int iState ;        //״̬
    int iQueryMovingCount ; //��ѯ�ƶ��ļ���

    QList <clsCx>  listCx ;
    int iAutoMode ;     //�Զ�����ģʽ
    int iAutoState ;    //�Զ�����״̬

    int iCxClid ;       //���߲������
    clsCx  currentCx ;  //��ǰ����
//    int iCxClState ;    //���߲���״̬
    bool bSecondHeightIng ; //�ڶ����������ڽ�����

signals:
    void sigFishPos( float * fPos, int iNums, bool bRes );  // bRes ��ʾ���ڲ����л��ǲ������
    void sigClRes( float * fCl, int iNums, bool bRes );       // false����������   true�����н��
    void sigSendMsg( QString );
    void sigHalf( );    //���Զ��׶���ɵ��ź�
    void sigAuto( ) ;   //�Զ�������ɣ�

};

#endif // QSERIALTHREAD_H
