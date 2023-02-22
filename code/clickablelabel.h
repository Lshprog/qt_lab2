#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT
 public:
     ClickableLabel(QWidget *parent=0): QLabel(parent){}
     ~ClickableLabel() {}
 signals:
     void clicked(ClickableLabel* click);
 protected:
     void mouseReleaseEvent(QMouseEvent*);
};

#endif // CLICKABLELABEL_H
