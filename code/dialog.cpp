#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::returnName()
{
    return ui->lineEdit->text();
}

QString Dialog::returnLink()
{
    return ui->lineEdit_2->text();
}

QString Dialog::returnDescription()
{
    return ui->lineEdit_3->text();
}

void Dialog::setName(QString data)
{
    ui->lineEdit->setText(data);
}

void Dialog::setLink(QString data)
{
    ui->lineEdit_2->setText(data);
}

void Dialog::setDescription(QString data)
{
    ui->lineEdit_3->setText(data);
}


void Dialog::on_buttonBox_accepted()
{
    accept();
}



void Dialog::on_buttonBox_rejected()
{
    reject();
}

