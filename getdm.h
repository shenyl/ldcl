#ifndef GETDM_H
#define GETDM_H

#include <QObject>
#include <QList>

//根据断面信息获取相应的值
//
class QGetDm : public QObject
{
    Q_OBJECT
public:
    explicit QGetDm(QObject *parent = 0);

    class clsDm{
    public:
        int iId;
        float fQdj;
        float fSs ;
    };

    float  getSs( float fWaterSurface, float fQdj );
    float getQdj(  float fWaterSurface, int iDir );

    float getQdjLeft( float fWaterSurface );
    float getQdjRight( float fWaterSurface );

    int getQdjLeftIndex( float fWaterSurface );
    int getQdjRightIndex( float fWaterSurface );

    QString getSysconfig( int iId );

    void readDm( );

private:
    QList< clsDm >  listDm ;
    float  fZeroOffset ;

signals:

public slots:

};

#endif // GETDM_H
