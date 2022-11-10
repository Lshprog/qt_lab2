#include "hyperlinkdelegate.h"
#include "QtCore/qglobal.h"
#include <QLabel>
#include <QDesktopServices>
#include <QEvent>
#include <QAbstractTextDocumentLayout>

HyperlinkDelegate::HyperlinkDelegate(QObject *parent):  QStyledItemDelegate(parent)
{

}

QSize HyperlinkDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option,index).expandedTo(QSize(64,option.fontMetrics.height() + 10));
}

void HyperlinkDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
   QStyleOptionViewItem opt = option;
   //initStyleOption(&opt, index);
//   Hyperlink *cur = static_cast<Hyperlink*>(index.internalPointer());
//   if(index.column()==0 && cur->getCategoryStatus())
//       opt.font.setBold(true);
//   else if(index.column() == 1){
//       opt.font.setUnderline(true);
//   }

   QStyledItemDelegate::paint(painter, opt, index);

}


bool HyperlinkDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(index.column() == 1 && event->type() == QEvent::MouseButtonPress){
        QMouseEvent *keyEvent = static_cast<QMouseEvent *>(event);
        if(keyEvent->button() == Qt::RightButton){
            QDesktopServices::openUrl(index.data().toString());
            return true;
        }
    }

    return false;
}


