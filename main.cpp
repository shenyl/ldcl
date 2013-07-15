#include "ldcl.h"

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include  <QFont>
#include  <QSqlDatabase>
#include  <QMessageBox>

#include  "qdlglogin.h"

int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE( ldcl );

    QApplication app(argc, argv);

    QFont f("ZYSong18030",16);
    app.setFont(f);//更改应用程序的默认字体

    QTextCodec* textCodec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForTr(textCodec);

    QSqlDatabase m_mainDB = QSqlDatabase::addDatabase("QSQLITE" );          //连接数据库
    QString strPath = QCoreApplication::applicationDirPath(  );

    m_mainDB.setDatabaseName( strPath + "/ldcl.cfg");

    bool bRes = m_mainDB.open( );

    if (!bRes) {
        QMessageBox::warning(NULL, textCodec->toUnicode("提示"), textCodec->toUnicode( "连接数据库失败！") );
        return -1;
    }

//    QDlgLogin dlgLogin ;

//    if ( dlgLogin.exec(  ) == QDialog::Accepted ){
//        if( dlgLogin.iResult != 1 ){
//            QMessageBox::warning(NULL, textCodec->toUnicode("提示"), textCodec->toUnicode( "密码错误!") );
//            return -1 ;
//        }
//    }
//    else
//        return -1 ;

    QLdcl ldcl ;
    ldcl.show();

    return app.exec();
}
