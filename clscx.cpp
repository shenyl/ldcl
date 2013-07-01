#include  "clscx.h"

#include  <QDebug>

//±¶Êý
static const double fMulti[4][5] = {
    1,0,0,0,0,
    1,1,0,0,0,
    1,1,1,0,0,
    1,3,3,2,1
};

//Ë®Éî
static const double fRatioSs[4][5] = {
    0.6, 0,0, 0, 0,
    0.2, 0.8, 0, 0, 0,
    0.2, 0.6, 0.8, 0, 0,
    0, 0.2, 0.6, 0.8, 1.0
};

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
