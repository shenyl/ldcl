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
#include <QComboBox>

#define   INDEX_CS_NO       0       //测深编号
#define   INDEX_CV_NO       1       //测速编号
#define   INDEX_QDJ         2       //起点距
#define   INDEX_WATER_GC    3       //水面高程
#define   INDEX_GC          4       //河底高程
#define   INDEX_YYSS        5       //应用水深

#define   INDEX_PJSS        6       //平均水深
#define   INDEX_JJ          7       //测深线间距
#define   INDEX_QXMJ        8       //测深垂线面积
#define   INDEX_LSAREA      9       //部分面积

#define   INDEX_LS          10       //测速线流速

#define   INDEX_LSPJ        11      //流速平均
#define   INDEX_LSQ         12      //部分流量

//流量计算结果类
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
    //流量制作类的成员
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
    QLabel * labelLsy ;
    QComboBox * pComboLsy ;

    QList<float> listHdgc ;     //河底高程
    QList<float> listYySs ;     //应用水深
    QList<float> listQdj ;      //起点距
    QList<float> listLs ;       //垂线流速

    QList<float> listAverSs ;   //测深平均水深
    QList<float> listAverJj ;   //测深线间距
    QList<float> listSsArea ;   //水深局部面积
    QList<int>  listLsIndex ;   //流速线序号  最左边的流速线与应用水深线为 0
    QList<float> listLsPj ;     //局部平均流速  第一个与最后一个是要乘以岸边系数
    QList<float> listLsArea ;   //流速局部面积
    QList<float> listLsQ ;     //流速流量

    float fQdjLeft, fQdjRight ;     //水面左右对应的起点距

    float fWaterGc ;
    float fLsParaLeft ;
    float fLsParaRight ;
    float fQdjOffset ;
    float fTj[20]   ;  //统计值

private:
    void getCsGc() ;    //获取测深高程
    void getVGc() ;    //获取测速高程
    void fillVNo( QString strQdj, int iNo, QString strLs );
    void fillArea( );
    void computerLs( );
    void fillTable( );
    QString sslr( float fValue, int iDigit );
    void tj(  );   //统计值
    void fillTj( );

    void saveXLS( );
    void InitComboLsy( );

};

#endif  // QDlgResultQQ_H
