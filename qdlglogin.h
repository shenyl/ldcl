#ifndef QDLGLOGIN_H
#define QDLGLOGIN_H

#include <QDialog>

class QLabel;
class QComboBox;
class QLineEdit;
class QPushButton;


class QDlgLogin : public QDialog
{
    Q_OBJECT

public:
    QDlgLogin(QWidget *parent = 0);
    virtual ~QDlgLogin();

    void initDialog();
    void fillComboOperator();

public slots:
    void pwdConfirm();
    void indexValue(int index);

public:
    int iResult ;

private:
    QLabel * pLabelName;
    QLabel *pLabelPwd;
    QComboBox *pNameList;
    QLineEdit *pPwdValue;
    QPushButton *pBtnOk;
    QPushButton *pBtnCancel;

    QStringList listId;
    QStringList listName;
    QStringList listPwd;
    QStringList listPower;

    //int iNumbers;
    int iNameListIndex;
};

#endif // QDLGLOGIN_H
