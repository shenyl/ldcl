#ifndef QSERIALTHREAD_H
#define QSERIALTHREAD_H

#include <QThread>
#include <QList>

#include "qcommserial.h"
#include "../clscx.h"
#include "../sendbuf.h"
#include "../getdm.h"

class QGetDm ;
class QRamDrive ;

#define  STATE_NOTSTART     -1

#define  STATE_HOR          0       //ˮƽ�ƶ�״̬
#define  STATE_HOR_OVER     1       //ˮƽ�ƶ�״̬����

#define  STATE_VER          2       //��ֱ�����ƶ�״̬
#define  STATE_VER_OVER     3       //���´�ֱ�ƶ�״̬����

#define  STATE_CL           4       //����״̬
#define  STATE_CL_OVER      5       //����״̬����

#define  STATE_VER_UP       6       //��������
#define  STATE_VER_UP_OVER  7       //�������̽���

#define  STATE_STOPUP       8       //��ͣ����

#define  CMD_HEAD   0   //����
#define  CMD_BACK   1   //�س�
#define  CMD_UP     2   //����
#define  CMD_DOWN   3   //����
#define  CMD_STOP   4   //ֹͣ

#define  CMD_CL     5   //����

#define  MODE_MANU  0   //�ֶ�
#define  MODE_HALF  1   //���Զ�
#define  MODE_AUTO  2   //ȫ�Զ�

#define  AUTO_STATE_START       1
#define  AUTO_STATE_PAUSE       2
#define  AUTO_STATE_STOP        3

//�����ڴ�����ֱ��
#define  POS_WATER              1
#define  POS_VOLTAGE            2
#define  POS_FREQUENCY          3

#define  KIND_NO            0
#define  KIND_SS_MANU       1       //�ֹ�����
#define  KIND_SS_EXPLORE    2       //����ˮλ�Ʋɼ�
#define  KIND_SS_CL         3       //����ˮ���źŲ���

#define  POS_WATER_BASE    9

class QSerialThread : public QThread
{
    Q_OBJECT

public:
    explicit QSerialThread(QObject *parent = 0);

    QSendBuf  sendBuf ;

    void init( );
    void run( );

    void setComNo( int iComNo ){ this->iComNo = iComNo; }
    bool setConfigQdj( QString strValue , bool bClear = true, bool bGo = true );
    bool setConfigSs( QString strValue , bool bClear = true , bool bGo = true );
    void sendCmdMove( int iCmd, bool bClear = true ,bool bGo = true );
    void sendCmdCl( );
    bool queryMove( );
    bool queryCl( );
    void stopUp( );

    void startAutoMode( int iMode );
    void stopAuto( );
    void pauseAuto( );
    void continueAuto( );
    void setHalfContinue( ){ bHalfContinue = true ; }
    void setStateNotStart( ){ iState = STATE_NOTSTART ;}
    void setStopUp( bool bStopUp ){ this->bStopUp = bStopUp ;}

    void clearCx( ){ listCx.clear( ); }
    void appendCx( clsCx  cx ){ listCx.append( cx ); }
    void printfCx();

    void runManu( );
    void runFullAuto( );

    void computerLs( );
    void moveFish(  );

    void resetQueryMovingCount( ){ iQueryMovingCount = 0; }
    int getMode( ){ return iAutoMode; }
    void  setRamDrive( QRamDrive * pRam  ){ this->pRam = pRam; }

    float getSs(  );

protected:
    void writeComm( char c );
    void readComm( );
    void readPara( );

private:
    QCommSerial serial ;
    QRamDrive * pRam ;

    bool bConnected ;           //�Ƿ�����
    int iComNo ;                //���ں�
    bool bHalfContinue ;        //���Զ����б�־
    bool bStopUp ;              //��ͣ���ᰴť

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
    bool bAux ;             //���Զ������ĸ����ڵ�
    bool bAfterClss  ;           //�Ƿ����ˮ��
    float  fWaterBase ;     //ˮλ��ֵ

    QGetDm  dm ;

signals:
    void sigFishPos( float * fPos, int iNums, bool bRes );  // bRes ��ʾ���ڲ����л��ǲ������
    void sigClRes( float * fCl, int iNums, bool bRes );       // false����������   true�����н��
    void sigSendMsg( QString );
    void sigHalf( );    //���Զ��׶���ɵ��ź�
    void sigAuto( QString strMsg ) ;   //�Զ�������ɣ�

};

#endif // QSERIALTHREAD_H
