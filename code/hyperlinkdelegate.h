#ifndef HYPERLINKDELEGATE_H
#define HYPERLINKDELEGATE_H

#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QObject>
#include <QPainter>
#include "hyperlink.h"


class HyperlinkDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit HyperlinkDelegate(QObject *parent = nullptr);
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);



};

#endif // HYPERLINKDELEGATE_H
