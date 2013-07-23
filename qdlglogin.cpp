#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDateTime>
#include <QTextCodec>

#include "qdlglogin.h"


QDlgLogin::QDlgLogin(QWidget *parent)
    : QDialog(parent), iNameListIndex(0)
{
    iPower = 0 ;
    initDialog();
    fillComboOperator();

    resize( 300, 170 );
    setWindowTitle(tr("��   ¼"));
}

QDlgLogin::~QDlgLogin()
{

}

void QDlgLogin::initDialog()
{
    QFont font(tr("����"), 15, QFont::Normal);
    setFont(font);  //�����������ڵ�����

    pLabelName = new QLabel(tr("��¼�ˣ�"));
//    pLabelName -> setFont(font);
	
    pNameList = new QComboBox;
    pLabelPwd = new QLabel(tr("��  �룺"));  

    pPwdValue = new QLineEdit;
    pPwdValue -> setEchoMode(QLineEdit::Password);

    pBtnOk = new QPushButton(tr("ȷ��"));
    pBtnCancel = new QPushButton(tr("�ر�"));

    connect(pBtnOk, SIGNAL(clicked()), this, SLOT(pwdConfirm()));          //ȷ��
    connect(pBtnCancel, SIGNAL(clicked()), this, SLOT(reject()));       //ȡ��
    connect(pNameList, SIGNAL(currentIndexChanged(int)), this, SLOT(indexValue(int)));

    //����
    QGridLayout * topLayout = new QGridLayout ;
    topLayout->addWidget( pLabelName, 0, 0  );
    topLayout->addWidget( pNameList,0, 1  );
    topLayout->addWidget( pLabelPwd, 1, 0 );
    topLayout->addWidget( pPwdValue, 1, 1 );

    QHBoxLayout *Layout1 = new QHBoxLayout;
    Layout1->addStretch();
    Layout1->addWidget(pBtnOk);
    Layout1->addWidget(pBtnCancel);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout -> addLayout(topLayout);
    mainLayout -> addLayout(Layout1);

    setLayout(mainLayout);
}

void QDlgLogin::fillComboOperator()
{
    QSqlQuery query;
    query.exec("SELECT operid, opername, pwd, operpower FROM operators");

    QString strOperName ;
    while(query.next())
    {
        listId.append(query.value(0).toString());

        strOperName = query.value(1).toString();
        pNameList->addItem( strOperName );

        listPwd.append(query.value(2).toString());
        listPower.append( query.value(3).toInt() );
    }
}

//��¼�û���������֤
void QDlgLogin::pwdConfirm()
{
    QString strPwdValue = pPwdValue->text( );
    if(strPwdValue == listPwd[iNameListIndex]){
        iResult = 1 ;
        iPower = listPower.at( iNameListIndex );
    }
    else
        iResult = 0 ;

    accept();
}

void QDlgLogin::indexValue(int index)
{
    iNameListIndex = index;
}

