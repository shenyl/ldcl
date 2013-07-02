#include "combodelegate.h"
#include <QComboBox>

ComboDelegate::ComboDelegate( int iEditKind, QString strEdit2,  QObject *parent) :
        QItemDelegate(parent)
{
    setMem( iEditKind, strEdit2 );
}

QWidget *ComboDelegate::createEditor(QWidget *parent,
const QStyleOptionViewItem &/*option*/,
const QModelIndex &/*index*/) const
{
    QComboBox *editor = new QComboBox(parent);

    for(int i=0; i< listString.size(); i++ ){
        editor->addItem( listString.at(i) );
    }

    editor->installEventFilter(const_cast<ComboDelegate*>(this));

    return editor;
}

void ComboDelegate::setEditorData(QWidget *editor,
                                  const QModelIndex &index) const
{
    int iValue =index.model()->data(index).toInt( );

    int iIndex = 0 ;
    for(int i=0; i<listInt.size() ; i++ ){
        if( iValue == listInt.at(i) ){
            iIndex = i ;
            break ;
        }
    }

    QComboBox *box = static_cast<QComboBox*>(editor);
    box->setCurrentIndex( iIndex );

}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QComboBox *box = static_cast<QComboBox*>(editor);
    int iIndex = box->currentIndex( );
    model->setData(index, listInt.at(iIndex) );
}


void ComboDelegate::updateEditorGeometry(QWidget *editor,
const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

void ComboDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    int iValue = index.model()->data(index,Qt::DisplayRole).toInt( );

    int iIndex = 0 ;
    for(int i=0; i<listInt.size(); i++ ){
        if( listInt.at(i) == iValue ) {
            iIndex = i;
            break ;
        }
    }

    QString text = listString.at(iIndex);

    QStyleOptionViewItem myOption = option;
    myOption.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;
    drawDisplay(painter,myOption,myOption.rect,text);
    drawFocus(painter,myOption,myOption.rect);
}

void ComboDelegate::setMem( int iEditKind, QString strEdit2 )
{
    this->iEditKind = iEditKind ;
    this->strEdit2 = strEdit2 ;

    if( iEditKind == 1 ){
        QStringList list1 = strEdit2.split(",", QString::SkipEmptyParts);
        QString  strItem ;
        for(int i=0; i<list1.size(); i++ ){
            strItem = list1.at(i) ;
            QStringList list2 = strItem.split("/", QString::SkipEmptyParts);
            listInt.append( list2.at(0).toInt() ) ;\
            listString.append(  list2.at(1).trimmed() );
        }
    }


}
