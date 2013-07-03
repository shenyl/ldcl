#include  "clscx.h"
#include  <QDebug>

clsCx::clsCx( )
{

}

clsCx::~clsCx( )
{

}

void clsCx::setMem( int iCxid, float fQdj, float fSs, int iClff, int iCzff )
{
    this->iCxid = iCxid ;
    this->fQdj = fQdj ;
    this->fSs = fSs ;
    this->iClff = iClff ;
    this->iCzff = iCzff ;
}

void clsCx::printf( )
{
    qDebug() << iCxid << fQdj << fSs << iClff << iCzff ;
}
