    #ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    QString returnName();
    QString returnLink();
    QString returnDescription();

    void setName(QString data);
    void setLink(QString data);
    void setDescription(QString data);

private slots:

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
