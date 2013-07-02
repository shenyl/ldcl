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
    app.setFont(f);//����Ӧ�ó����Ĭ������

    QTextCodec* textCodec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForTr(textCodec);

//    //�������ݿ�
//    QSqlDatabase m_mainDB = QSqlDatabase::addDatabase("QSQLITE" );          //�������ݿ�
//    QString strPath = QCoreApplication::applicationDirPath(  );
//    m_mainDB.setDatabaseName( strPath + "/ldclaaa.cfg");

//    bool bRes = m_mainDB.open( );

//    if ( !bRes ) {
//        QMessageBox::warning( NULL, textCodec->toUnicode( "�������ݿ�ʧ�ܣ�" ), \
//        textCodec->toUnicode( "�������ݿ�ʧ�ܣ�") );
//    }

    QLdcl ldcl ;
    ldcl.show();

    return app.exec();
}
