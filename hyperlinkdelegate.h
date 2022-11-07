#ifndef HYPERLINKDELEGATE_H
#define HYPERLINKDELEGATE_H

#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QObject>
#include <QPainter>



class HyperlinkDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit HyperlinkDelegate(QObject *parent = nullptr);




    // QAbstractItemDelegate interface
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // HYPERLINKDELEGATE_H
