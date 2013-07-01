#ifndef CLSCX_H
#define CLSCX_H

class clsCx
{
public:
    clsCx( );
    ~clsCx( );

    int iNo ;           //编号
    float fQdj ;        //起点距
    float fSs ;         //水深

    int iClff ;         //测量方法  0/1点法,1/2点法,2/3点法,3/5点法
    int iCzff ;         //操作方法  0/不操作,1/测速 2/测深与测速
    float fCxLs ;       //计算出的垂线流速
    float fDotLs[5] ;   //测量点的流速
};

#endif // CLSCX_H
