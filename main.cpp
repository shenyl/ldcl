#include "ldcl.h"

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include  <QFont>
#include  <QSqlDatabase>
#include  <QMessageBox>

int main(int argc, char* argv[])
{
    Q_INIT_RESOURCE( ldcl );

    QApplication app(argc, argv);

    QFont f("ZYSong18030",16);
    app.setFont(f);//更改应用程序的默认字体

    QTextCodec* textCodec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForTr(textCodec);

//    //联接数据库
//    QSqlDatabase m_mainDB = QSqlDatabase::addDatabase("QSQLITE" );          //连接数据库
//    QString strPath = QCoreApplication::applicationDirPath(  );
//    m_mainDB.setDatabaseName( strPath + "/ldclaaa.cfg");

//    bool bRes = m_mainDB.open( );

//    if ( !bRes ) {
//        QMessageBox::warning( NULL, textCodec->toUnicode( "连接数据库失败！" ), \
//        textCodec->toUnicode( "连接数据库失败！") );
//    }

    QLdcl ldcl ;
    ldcl.show();

    return app.exec();
}
