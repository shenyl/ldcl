#ifndef CLSCX_H
#define CLSCX_H

//�������ټ��㷽��
//һ�㷨	Vm = V0.6
//���㷨	Vm = ( V0.2 + V0.8 ) / 2
//���㷨	Vm = ( V0.2 + V0.6 + V0.8) / 3
//��㷨	Vm = (V0.0 + 3V0.2 + 3V0.6 + 2V0.8 + V1.0) / 10

//aaaa

class clsCx
{
public:
    clsCx( );
    ~clsCx( );

    void setMem( int iCxid, float fQdj, float fSs, int iClff, int iCzff );
    void printf(  );

    int iCxid ;          //���
    float fQdj ;        //����
    float fSs ;         //ˮ��
    int iClff ;         //��������  0/1�㷨,1/2�㷨,2/3�㷨,3/5�㷨
    int iCzff ;         //��������  0/������,1/���� 2/���������

    float fCxLs ;       //������Ĵ�������
    float fDotLs[5] ;   //�����������

    float fT[5];    //��ʱ
    float fV[5];    //�ٶ�
    float fN[5];    //��ת��

    float fCdSs[5];    //���ˮ��

    float fK, fC ;  //����ʱ�ĳ���

};

#endif // CLSCX_H
