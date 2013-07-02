#ifndef COMBODELEGATE_H
#define COMBODELEGATE_H

#include <QItemDelegate>

#include <QList>

class ComboDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ComboDelegate( int iEditKind = 0 , QString strEdit2 = tr(""), QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    void paint(QPainter *painter,const QStyleOptionViewItem &option, const QModelIndex& index)const;

    void setMem( int iEditKind, QString strEdit2 );
private:
    int iEditKind ;
    QString strEdit2 ;

    QStringList listString ;
    QList< int  > listInt ;
};

#endif // COMBODELEGATE_H
