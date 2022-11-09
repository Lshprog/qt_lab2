#include "hyperlinkdelegate.h"
#include "QtCore/qglobal.h"
#include <QLabel>
#include <QDesktopServices>
#include <QEvent>

HyperlinkDelegate::HyperlinkDelegate(QObject *parent):  QStyledItemDelegate(parent)
{

}


//QWidget *HyperlinkDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
//{
//    if(index.column() == 1){
//        ClickableLabel *hyplink = new ClickableLabel(parent);
//        //hyplink->setText("<a href=\""+ index.data().toString()+"\">"+index.data().toString()+"</a>");

//        qDebug()<<"At least soemthing11";
//        hyplink->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
//        hyplink->setStyleSheet("QLabel{ background-color : transparent;font-weight: bold}");
//        hyplink->setOpenExternalLinks(true);
//        return hyplink;
//    }
//    else{
//        return QStyledItemDelegate::createEditor(parent,option,index);
//        qDebug()<<"At least soemthing11";
//    }
//}

//void HyperlinkDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
//{
//    if(index.column() == 1){
//        ClickableLabel *hyplink = static_cast<ClickableLabel*>(editor);
//        QString link = index.model()->data(index,Qt::DisplayRole).toString();
//        hyplink->setText(link);
//        hyplink->setStyleSheet("QLabel{ background-color : transparent;font-weight: bold}");;
//        hyplink->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
//        hyplink->setOpenExternalLinks(true);

//        qDebug()<<"At least soemthing";

//    }
//    else
//        QStyledItemDelegate::setEditorData(editor,index);
//}

//void HyperlinkDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
//{
//    if(index.column() == 1){
//        ClickableLabel *hyplink = static_cast<ClickableLabel*>(editor);

//        hyplink->setStyleSheet("QLabel{ background-color : transparent;font-weight: bold}");

//        model->setData(index,QVariant::fromValue(hyplink->text()),Qt::EditRole);
//        hyplink->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
//        hyplink->setOpenExternalLinks(true);
//        qDebug()<<"/////At least soemthing !!!!!!!!!";

//    }
//    else{
//        QStyledItemDelegate::setModelData(editor,model,index);
//    }

//}

void HyperlinkDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect);

}

QSize HyperlinkDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QStyledItemDelegate::sizeHint(option,index).expandedTo(QSize(64,option.fontMetrics.height() + 10));
}

void HyperlinkDelegate::paint(QPainter *painter,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    if(index.column() ==1){

        QStyleOptionViewItem opt(option);
        ClickableLabel *label = new ClickableLabel;
        label->setText("<a href=\""+ index.data().toString() +"\">"+index.data().toString()+"</a>");
        //label->setTextFormat(Qt::RichText);
        //label->setText(index.data().toString());
        //label->setTextInteractionFlags(Qt::LinksAccessibleByMouse);
        //label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        label->setOpenExternalLinks(true);

        label->setGeometry(option.rect);
        label->setStyleSheet("QLabel { background-color : transparent; }");
        painter->translate(option.rect.topLeft());
        label->render(painter);
        painter->translate(-option.rect.topLeft());

    }
    else if(index.column() == 0){
        QStyleOptionViewItem opt(option);
        Hyperlink * currenthyp = static_cast<Hyperlink*>(index.internalPointer());
        QLabel *label = new QLabel;
        label->setText(index.data().toString());

        label->setStyleSheet("QLabel { background-color : transparent;}");

        if(currenthyp->getCategoryStatus()){
            label->setStyleSheet("QLabel{ background-color : transparent;font-weight: bold}");
            qDebug()<<"Rabotaet";
        }
        label->setGeometry(option.rect);
        painter->translate(option.rect.topLeft());
        label->render(painter);
        painter->translate(-option.rect.topLeft());
    }
    else {
        QStyledItemDelegate::paint(painter,option,index);
    }
}


bool HyperlinkDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(index.column() == 1 && event->type() == QEvent::MouseButtonDblClick){
        QDesktopServices::openUrl(index.data().toString());
        return true;
    }
    return false;
}
