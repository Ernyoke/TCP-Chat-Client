#include "errormessage.h"
#include "ui_errormessage.h"

ErrorMessage::ErrorMessage(QString text, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorMessage)
{
    ui->setupUi(this);
    ui->label->setText(text);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(okPressed()));
}

ErrorMessage::~ErrorMessage()
{
    delete ui;
}

void ErrorMessage::okPressed() {
    //this->close();
    emit destr();
}
