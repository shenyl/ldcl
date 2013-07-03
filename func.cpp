#include "func.h"

#include <QtGui/QApplication>

#include <QFileDialog>
#include <QObject>
#include <QDebug>
#include <QAxObject>
#include <QMessageBox>

#include <QSqlRecord>
#include <QSqlField>

#define  APP_JBCL1   1
#define  APP_JBJL2   2

QFunc::QFunc(QObject *parent) :
    QObject(parent)
{

}

//联接数据库文件
void QFunc::connectDatabase( int iAppid )
{
    QSqlDatabase m_mainDB = QSqlDatabase::addDatabase("QSQLITE" );          //连接数据库
    QString strPath = QCoreApplication::applicationDirPath(  );

    if(iAppid == APP_JBCL1)
        m_mainDB.setDatabaseName( strPath + "/jbcl.db");
    if(iAppid == APP_JBJL2)
        m_mainDB.setDatabaseName( strPath + "/jbjl.db");

    bool bRes = m_mainDB.open( );

    if ( !bRes ) {
        QMessageBox::warning(NULL, tr("连接数据库失败！"), tr("连接数据库失败！"));
        return;
    }
}

//QTableWidget * pTable 表控件
//QStringList listLen  表各个字段的宽度
void QFunc::saveXLS( QTableWidget * pTable, QStringList listLen )
{
    QFileDialog saveFileDlg( 0, tr("保存记录文件"), "./save", tr("Excel files(*.xlsx)"));
    saveFileDlg.setAcceptMode(QFileDialog::AcceptSave);
    saveFileDlg.setDefaultSuffix(QString("xlsx"));
//    saveFileDlg.selectFile(strName);  //预置一个文件名字

    QString strPath;
    if (saveFileDlg.exec()) {
        strPath = saveFileDlg.selectedFiles().at(0);
    }

    qDebug( ) << strPath ;

    if (strPath.isEmpty())  return;

    strPath = strPath.replace("/", "\\");

    qDebug( ) << strPath ;

    QAxObject excelObj("Excel.Application");
    QAxObject* excelWorkBooks = excelObj.querySubObject("Workbooks");
    QAxObject* excelWorkBook;
    QAxObject* excelSheets;

    if (excelWorkBooks) {
        QFile file(strPath);
        if (file.exists())
            excelWorkBook = excelWorkBooks->querySubObject("Open(const QString&)", strPath);
        else
            excelWorkBook = excelWorkBooks->querySubObject("Add()");

        if (excelWorkBook)
            excelSheets = excelWorkBook->querySubObject("Sheets");
         else
            QMessageBox::information(0, "", "QAxObject workbook fail!");
    }
    else
        QMessageBox::information(0, "", "初始化Excel错误,可能没有安装Office组件!");


    QAxObject* excelWorkSheet = excelSheets->querySubObject("Item(int index)", 1);

    QAxObject* range;
    QAxObject* interior;
    QAxObject* font;

    QTableWidgetItem* pItem;
    QString strText;

    //填充台头
    for (int i=0; i< pTable->columnCount(); ++i) {
        pItem = pTable->horizontalHeaderItem(i);
        strText = pItem->text();
        range = excelWorkSheet->querySubObject("Cells(int, int)", 1, i+1);
        range->dynamicCall("SetValue(const QVariant&)", QVariant(strText));
        range->setProperty("HorizontalAlignment", 3);
        range->setProperty("ColumnWidth", listLen.at(i).toInt() );

        interior = range->querySubObject("interior");
        interior->setProperty("ColorIndex", QVariant(8));

        font = range->querySubObject("font");
        font->setProperty("ColorIndex", QVariant(0));
        font->setProperty("Bold", true);
    }

    //填充内容
    for (int i=0; i<pTable->rowCount(); ++i) {
        for (int j=0; j < pTable->columnCount(); ++j) {
            pItem = pTable->item(i, j);
            strText = pItem->text();
            range = excelWorkSheet->querySubObject("Cells(int, int)", i+2, j+1);
            range->dynamicCall("SetValue(const QVariant&)", QVariant(strText));
            range->setProperty("HorizontalAlignment", 2);                     //靠左 2 中央 3  靠右4

            // range->setProperty("NumberFormatLocal", "yyyy-mm-dd hh:mm:ss;@"); //时间格式
        }
    }

    //保存关闭excel表格
    excelObj.setProperty("DisplayAlerts", 0);
    excelWorkBook->dynamicCall("SaveAs(const QString&)", strPath);
    excelWorkBook->dynamicCall("Close(Boolean)", false);

    excelObj.dynamicCall("Quit()");
}


void QFunc::saveXLS( QTableView * pTable, QSqlQueryModel *tabModel )
{
    QFileDialog saveFileDlg( 0, tr("保存记录文件"), "./", tr("Excel files(*.xls)"));
    saveFileDlg.setAcceptMode(QFileDialog::AcceptSave);
    saveFileDlg.setDefaultSuffix(QString("xls"));
//    saveFileDlg.selectFile(strName);  //预置一个文件名字

    QString strPath;
    if (saveFileDlg.exec()) {
        strPath = saveFileDlg.selectedFiles().at(0);
    }

    qDebug( ) << strPath ;

    if (strPath.isEmpty())  return;

    strPath = strPath.replace("/", "\\");

    qDebug( ) << strPath ;

    QAxObject excelObj("Excel.Application");
    QAxObject* excelWorkBooks = excelObj.querySubObject("Workbooks");
    QAxObject* excelWorkBook;
    QAxObject* excelSheets;

    if (excelWorkBooks) {
        QFile file(strPath);
        if (file.exists())
            excelWorkBook = excelWorkBooks->querySubObject("Open(const QString&)", strPath);
        else
            excelWorkBook = excelWorkBooks->querySubObject("Add()");

        if (excelWorkBook)
            excelSheets = excelWorkBook->querySubObject("Sheets");
         else
            QMessageBox::information(0, "", "QAxObject workbook fail!");
    }
    else
        QMessageBox::information(0, "", "初始化Excel错误,可能没有安装Office组件!");


    QAxObject* excelWorkSheet = excelSheets->querySubObject("Item(int index)", 1);

    QAxObject* range;
    QAxObject* interior;
    QAxObject* font;

    QString strText;

    //填充台头
    for (int i=0; i< tabModel->columnCount(); i++) {
        strText= tabModel->headerData( i, Qt::Horizontal ).toString();
//        qDebug()  << strText ;
        range = excelWorkSheet->querySubObject("Cells(int, int)", 1, i+1);
        range->dynamicCall("SetValue(const QVariant&)", QVariant(strText));
        range->setProperty("HorizontalAlignment", 3);
        range->setProperty("ColumnWidth", pTable->columnWidth( i )/7 );

        interior = range->querySubObject("interior");
        interior->setProperty("ColorIndex", QVariant(8));

        font = range->querySubObject("font");
        font->setProperty("ColorIndex", QVariant(0));
        font->setProperty("Bold", true);
    }

    int iRows ;
    iRows = tabModel->rowCount( );
    qDebug( ) << iRows ;

    QSqlRecord record ;

    //填充内容
    for (int i=0; i<tabModel->rowCount( ); ++i) {
        record = tabModel->record( i );
        for (int j=0; j < tabModel->columnCount(); j++) {
            range = excelWorkSheet->querySubObject("Cells(int, int)", i+2, j+1);
            range->dynamicCall("SetValue(const QVariant&)", record.value(j) );
            range->setProperty("HorizontalAlignment", 2);                     //靠左 2 中央 3  靠右4

            // range->setProperty("NumberFormatLocal", "yyyy-mm-dd hh:mm:ss;@"); //时间格式
        }
    }

    //保存关闭excel表格
    excelObj.setProperty("DisplayAlerts", 0);
    excelWorkBook->dynamicCall("SaveAs(const QString&)", strPath);
    excelWorkBook->dynamicCall("Close(Boolean)", false);

    excelObj.dynamicCall("Quit()");
}
