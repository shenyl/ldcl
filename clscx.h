#ifndef CLSCX_H
#define CLSCX_H

class clsCx
{
public:
    clsCx( );
    ~clsCx( );

    int iNo ;           //���
    float fQdj ;        //����
    float fSs ;         //ˮ��

    int iClff ;         //��������  0/1�㷨,1/2�㷨,2/3�㷨,3/5�㷨
    int iCzff ;         //��������  0/������,1/���� 2/���������
    float fCxLs ;       //������Ĵ�������
    float fDotLs[5] ;   //�����������
};

#endif // CLSCX_H
