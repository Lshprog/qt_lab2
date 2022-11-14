#include "hyperlinkdelegate.h"
#include "QtCore/qglobal.h"
#include <QLabel>
#include <QDesktopServices>
#include <QEvent>
#include <QAbstractTextDocumentLayout>
#include <QMessageBox>

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
   QStyledItemDelegate::paint(painter, opt, index);
}


bool HyperlinkDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(index.column() == 1 && event->type() == QEvent::MouseButtonPress){
        QMouseEvent *keyEvent = static_cast<QMouseEvent *>(event);
        if(keyEvent->button() == Qt::RightButton){
            QUrl url = QUrl::fromUserInput(index.data().toString());
            bool result = QDesktopServices::openUrl(url);
            if(result)
                return true;
            else{
                QMessageBox msgBox;
                msgBox.setText("Warning!");
                msgBox.setInformativeText("Incorrect link!");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setIcon(QMessageBox::Warning);
                int ret = msgBox.exec();
                return false;
            }
        }

    }

    return false;
}


