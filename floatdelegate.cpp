#include "floatdelegate.h"
#include <QComboBox>
#include <QLineEdit>

FloatDelegate::FloatDelegate( int iEditKind, QString strEdit2,  QObject *parent) :
        QItemDelegate(parent)
{
    setMem( iEditKind, strEdit2 );
}

QWidget *FloatDelegate::createEditor(QWidget *parent,
const QStyleOptionViewItem &/*option*/,
const QModelIndex &/*index*/) const
{
    QLineEdit * pEdit = new QLineEdit(parent);

    pEdit->installEventFilter(const_cast<FloatDelegate*>(this));

    return pEdit;
}

void FloatDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    float fV = index.model()->data(index).toFloat( );

    QLineEdit * pEdit = static_cast<QLineEdit*>(editor);
    pEdit->setText( QString("%1").arg(fV) );
}

void FloatDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QLineEdit * pEdit = static_cast<QLineEdit*>(editor);

    model->setData(index, pEdit->text().toFloat( ) );
}


void FloatDelegate::updateEditorGeometry(QWidget *editor,
const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

//void FloatDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const
//{
//    int iValue = index.model()->data(index,Qt::DisplayRole).toInt( );

//    int iIndex = 0 ;
//    for(int i=0; i<listInt.size(); i++ ){
//        if( listInt.at(i) == iValue ) {
//            iIndex = i;
//            break ;
//        }
//    }

//    QString text = listString.at(iIndex);

//    QStyleOptionViewItem myOption = option;
//    myOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
//    drawDisplay(painter,myOption,myOption.rect,text);
//    drawFocus(painter,myOption,myOption.rect);
//}

void FloatDelegate::setMem( int iEditKind, QString strEdit2 )
{
    this->iEditKind = iEditKind ;
    this->strEdit2 = strEdit2 ;

//    if( iEditKind == 1 ){
//        QStringList list1 = strEdit2.split(",", QString::SkipEmptyParts);
//        QString  strItem ;
//        for(int i=0; i<list1.size(); i++ ){
//            strItem = list1.at(i) ;
//            QStringList list2 = strItem.split("/", QString::SkipEmptyParts);
//            listInt.append( list2.at(0).toInt() ) ;\
//            listString.append(  list2.at(1).trimmed() );
//        }
//    }


}
