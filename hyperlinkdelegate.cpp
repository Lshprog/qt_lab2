#include "hyperlinkdelegate.h"
#include "QtCore/qglobal.h"
#include <QLabel>

HyperlinkDelegate::HyperlinkDelegate(QObject *parent):  QStyledItemDelegate(parent)
{

}

QWidget *HyperlinkDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.column() == 1){
        QLabel *hyplink = new QLabel(parent);
        hyplink->setText("<a href=\" "+ index.data().toString() +"\">"+index.data().toString()+"</a>");
        hyplink->setOpenExternalLinks(true);
        return hyplink;
    }
    else
        return QStyledItemDelegate::createEditor(parent,option,index);
}

void HyperlinkDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if(index.column() == 1){
        QLabel *hyplink = static_cast<QLabel*>(editor);
        QString link = index.model()->data(index,Qt::DisplayRole).toString();
        hyplink->setText("<a href=\" "+ link +"\">"+link+"</a>");
        hyplink->setOpenExternalLinks(true);

    }
    else
        QStyledItemDelegate::setEditorData(editor,index);
}

void HyperlinkDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if(index.column() == 1){
        QLabel *hyplink = static_cast<QLabel*>(editor);
        model->setData(index,hyplink->text(),Qt::EditRole);
    }
    else{
        QStyledItemDelegate::setModelData(editor,model,index);
    }

}

void HyperlinkDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect);




}

QSize HyperlinkDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option,index).expandedTo(QSize(64,option.fontMetrics.height() + 10));
}
