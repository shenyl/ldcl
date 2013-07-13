#ifndef QDlgResultQ_H
#define QDlgResultQ_H

#include <QMdiSubWindow>

#include <QDialog>
#include <QSqlTableModel>
#include <QPushButton>
#include <QTableView>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QSqlQueryModel>
#include <QTableWidget>
#include <QCheckBox>
#include <QList>

#define   INDEX_CS_NO       0       //������
#define   INDEX_CV_NO       1       //���ٱ��
#define   INDEX_QDJ         2       //����
#define   INDEX_WATER_GC    3       //ˮ��߳�
#define   INDEX_GC          4       //�ӵ׸߳�
#define   INDEX_YYSS        5       //Ӧ��ˮ��

#define   INDEX_PJSS        6       //ƽ��ˮ��
#define   INDEX_JJ          7       //���
#define   INDEX_QXMJ        8       //�������
#define   INDEX_LSAREA      9       //�������

#define   INDEX_LS          10       //����

#define   INDEX_LSPJ        11      //����ƽ��
#define   INDEX_LSQ         12      //�ֲ�����

//������������
class QDlgResultQ : public QDialog
{
    Q_OBJECT
public:
    QDlgResultQ( QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~QDlgResultQ( );

    QString formatTime( QString strTime );
    void setTitleQ(  );

private slots:
    void slotQueryCl( );
    void slotMakeQ( );
    void slotMakeReport( );
    void slotSelectChange( int );

private:
    //����������ĳ�Ա
    QLabel * lblStart, * lblEnd ;
    QLineEdit * edtDateStart, * edtDateEnd ;
    QTableWidget * pTableCl, * pTableQ ;
    QPushButton *buttonQueryCl ;
    QCheckBox * checkBoxSelect ;
    QPushButton *buttonSelectAll ;
    QPushButton *buttonUnselectAll ;

    QLabel * lblGcStart, * lblGcEnd ;
    QLineEdit * edtGcStart, * edtGcEnd ;
    QPushButton *buttonMakeQ ;
    QPushButton *buttonMakeReport ;

    QList<float> listYySs ;
    QList<float> listQdj ;
    QList<float> listLs ;       //��������

    QList<float> listSsArea ;   //ˮ��ֲ����
    QList<int>  listLsIndex ;   //���������  ����ߵ���������Ӧ��ˮ����Ϊ 0
    QList<float> listLsPj ;     //�ֲ�ƽ������  ��һ�������һ����Ҫ���԰���ϵ��
    QList<float> listLsArea ;     //���پֲ����
    QList<float> listLsQ ;     //��������

    float fWaterGc ;
    float fLsParaLeft ;
    float fLsParaRight ;
    float fQdjOffset ;

private:
    void getCsGc() ;    //��ȡ����߳�
    void getVGc() ;    //��ȡ���ٸ߳�
    void fillVNo( QString strQdj, int iNo, QString strLs );
    void fillArea( );
    void computerLs( );
    void fillTable( );

};

#endif  // QDlgResultQQ_H
