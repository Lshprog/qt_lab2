#ifndef MSGBOXC_H
#define MSGBOXC_H

#include <QMessageBox>
#include <QDialog>

namespace Ui{
class MsgBoxC;
}

class MsgBoxC : public QMessageBox
{
    Q_OBJECT

public:
    MsgBoxC(QWidget *parent = nullptr);

private:
    Ui::MsgBoxC *ui;

};

#endif // MSGBOXC_H
