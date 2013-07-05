#ifndef CLSCX_H
#define CLSCX_H

//垂线流速计算方法
//一点法	Vm = V0.6
//二点法	Vm = ( V0.2 + V0.8 ) / 2
//三点法	Vm = ( V0.2 + V0.6 + V0.8) / 3
//五点法	Vm = (V0.0 + 3V0.2 + 3V0.6 + 2V0.8 + V1.0) / 10

//aaaa

class clsCx
{
public:
    clsCx( );
    ~clsCx( );

    void setMem( int iCxid, float fQdj, float fSs, int iClff, int iCzff );
    void printf(  );

    int iCxid ;          //编号
    float fQdj ;        //起点距
    float fSs ;         //水深
    int iClff ;         //测量方法  0/1点法,1/2点法,2/3点法,3/5点法
    int iCzff ;         //操作方法  0/不操作,1/测速 2/测深与测速

    float fCxLs ;       //计算出的垂线流速
    float fDotLs[5] ;   //测量点的流速

    float fT[5];    //历时
    float fV[5];    //速度
    float fN[5];    //总转数

    float fCdSs[5];    //测点水深

    float fK, fC ;  //测量时的常数

};

#endif // CLSCX_H
