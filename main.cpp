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
    app.setFont(f);//����Ӧ�ó����Ĭ������

    QTextCodec* textCodec = QTextCodec::codecForLocale();
    QTextCodec::setCodecForTr(textCodec);

    QSqlDatabase m_mainDB = QSqlDatabase::addDatabase("QSQLITE" );          //�������ݿ�
    QString strPath = QCoreApplication::applicationDirPath(  );

    m_mainDB.setDatabaseName( strPath + "/ldcl.cfg");

    bool bRes = m_mainDB.open( );

    if (!bRes) {
        QMessageBox::warning(NULL, textCodec->toUnicode("��ʾ"), textCodec->toUnicode( "�������ݿ�ʧ�ܣ�") );
        return -1;
    }

//    QDlgLogin dlgLogin ;

//    if ( dlgLogin.exec(  ) == QDialog::Accepted ){
//        if( dlgLogin.iResult != 1 ){
//            QMessageBox::warning(NULL, textCodec->toUnicode("��ʾ"), textCodec->toUnicode( "�������!") );
//            return -1 ;
//        }
//    }
//    else
//        return -1 ;

    QLdcl ldcl ;
    ldcl.show();

    return app.exec();
}
