#ifndef RAMDRIVE_H
#define RAMDRIVE_H

#include <QSharedMemory>

//共享数据区使用类

#define		MAX_DATA_SIZE			2000
#define		MAX_STRUCT_NUM_UP		25
#define		MAX_STRUCT_NUM_DOWN		25

struct StructDrive
{
        int iVersion ;                          //版本值
        int iHeartBreak ;                       //心跳计数

        double  dValue[MAX_DATA_SIZE] ;         //数据值
        char  chState[MAX_DATA_SIZE];		//数据状态 1  正常状态    2 非正常状态
	
        int iStructNumUp ;			//上行结构数据个数  当没有时为0 当存在时为具体的数目
        char chStructUp[ MAX_STRUCT_NUM_UP ][ 200 ];  //上行结构数据文本

        int iStructNumDown ;                    //下行结构数据个数  当没有时为0 当存在时为具体的数目
        char chStructDown[ MAX_STRUCT_NUM_DOWN ][ 200 ];    //下行结构数据文本

        char memo[100];                          //100个备用字符
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
