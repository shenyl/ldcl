#include "ramdrive.h"

QRamDrive::QRamDrive()
{
    drive = NULL ;
    bRam = false ;
}

bool QRamDrive::CreateRam( int ramid ){
    this->iRamid = ramid ;
    shareMem.setKey( QString("RAM%1").arg( ramid )  );
    bRam = shareMem.create( sizeof( StructDrive ) );
    int iRes;
    iRes = shareMem.error();
    switch (iRes)
    {
        case 0:
            drive = ( StructDrive * ) shareMem.data( );
            break;
        case 4:
            bRam = shareMem.attach( );
            drive = ( StructDrive * ) shareMem.data( );
            break;
    }
    if( bRam ){
        drive->iVersion = 0 ;
        drive->iHeartBreak = 0 ;
    }
    return bRam;
}

void QRamDrive::SetValue( int id, double dValue ){
        if( drive == NULL ) return ;
        drive->dValue[id] = dValue ;
}

void QRamDrive::SetState( int id, char chState )
{
    if( drive == NULL ) return ;
    drive->chState[id] = chState ;
}

double QRamDrive::GetValue( int id ){
        if( drive == NULL ) return 0;
        return  drive->dValue[ id ];
}

char  QRamDrive::GetState( int id )
{
    if( drive == NULL ) return 0 ;
    return  drive->chState[ id ];
}

void QRamDrive::SetStructUp( int iNum , char * chMsgUp )
{
    if ( drive == NULL ) return  ;
    memset( drive->chStructUp[iNum], 0, 200 );
    strcpy( *( drive->chStructUp + iNum ), chMsgUp );
}

void QRamDrive::SetStructDown( int iNum , char * chMsgDown )
{
    if ( drive == NULL ) return  ;
    memset( drive->chStructDown[iNum], 0, 200 );
    strcpy( *( drive->chStructDown + iNum ), chMsgDown );
}

char * QRamDrive::GetStructUp( int iNum ){
    if ( drive == NULL) return NULL ;
    return *( drive->chStructUp + iNum ) ;
}

char * QRamDrive::GetStructDown( int iNum ){
    if ( drive == NULL) return NULL ;
    return *( drive->chStructDown + iNum );
}

void QRamDrive::addMsgUp( char * chMsg )
{
    int iNum = drive->iStructNumUp;
    SetStructUp( iNum, chMsg );
    SetStructNumUp( iNum + 1 );
}

void QRamDrive::addMsgDown( char * chMsg )
{
    int iNum = drive->iStructNumDown;
    SetStructDown( iNum, chMsg );
    SetStructNumDown( iNum + 1 );
}
