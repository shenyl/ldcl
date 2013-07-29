#ifndef RAMDRIVE_H
#define RAMDRIVE_H

#include <QSharedMemory>

//����������ʹ����

#define		MAX_DATA_SIZE			2000
#define		MAX_STRUCT_NUM_UP		25
#define		MAX_STRUCT_NUM_DOWN		25

struct StructDrive
{
        int iVersion ;                          //�汾ֵ
        int iHeartBreak ;                       //��������

        double  dValue[MAX_DATA_SIZE] ;         //����ֵ
        char  chState[MAX_DATA_SIZE];		//����״̬ 1  ����״̬    2 ������״̬
	
        int iStructNumUp ;			//���нṹ���ݸ���  ��û��ʱΪ0 ������ʱΪ�������Ŀ
        char chStructUp[ MAX_STRUCT_NUM_UP ][ 200 ];  //���нṹ�����ı�

        int iStructNumDown ;                    //���нṹ���ݸ���  ��û��ʱΪ0 ������ʱΪ�������Ŀ
        char chStructDown[ MAX_STRUCT_NUM_DOWN ][ 200 ];    //���нṹ�����ı�

        char memo[100];                          //100�������ַ�
};


class QRamDrive
{
public:
    QRamDrive();

    bool CreateRam( int ramid );
    int getRam( ){ return iRamid; }

    bool GetRamState() {  return drive != NULL ; }

    void InsHeartBreak(){ drive->iHeartBreak ++ ; }
    void resetHeartBreak(){ drive->iHeartBreak = 0; }
    int getHeartBreak(){ return drive->iHeartBreak; }

    void SetValue( int id, double dValue );
    void SetState( int id, char chState );

    double GetValue( int id );
    char  GetState( int id );

    void SetStructNumUp( int iStructNum ){ drive->iStructNumUp = iStructNum; }
    int GetStructNumUp(  ){ return drive->iStructNumUp;  }

    int GetStructNumDown( ){ return drive->iStructNumDown ; }
    void SetStructNumDown( int iNum ){ drive->iStructNumDown = iNum  ; }

    void SetStructUp( int iNum , char * chMsgUp );
    void SetStructDown( int iNum , char * chMsgDown );

    char * GetStructUp( int iNum );
    char * GetStructDown( int iNum );

    void addMsgUp(char * chMsg);
    void addMsgDown(char * chMsg);

private:
    QSharedMemory  shareMem;
    StructDrive    * drive;
    bool bRam ;
    int iRamid;

};

#endif // RAMDRIVE_H
