#include "dialogcat.h"
#include "ui_dialogcat.h"

DialogCat::DialogCat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCat)
{
    ui->setupUi(this);
}

DialogCat::~DialogCat()
{
    delete ui;
}


QString DialogCat::returnName()
{
    return ui->lineEdit->text();
}

QString DialogCat::returnDescription()
{
    return ui->lineEdit_2->text();
}

void DialogCat::setName(QString data)
{
    ui->lineEdit->setText(data);
}

void DialogCat::setDescription(QString data)
{
    ui->lineEdit_2->setText(data);
}


void DialogCat::on_buttonBox_accepted()
{
    accept();
}



void DialogCat::on_buttonBox_rejected()
{
    reject();
}
