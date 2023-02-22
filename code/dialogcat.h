#ifndef DIALOGCAT_H
#define DIALOGCAT_H

#include <QDialog>

namespace Ui {
class DialogCat;
}

class DialogCat : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCat(QWidget *parent = nullptr);
    ~DialogCat();

    QString returnName();
    QString returnDescription();

    void setName(QString data);
    void setDescription(QString data);

private slots:

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
private:
    Ui::DialogCat *ui;
};

#endif // DIALOGCAT_H
