#include "progress.h"
#include "ui_progress.h"
#include <QDebug>

Progress::Progress(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Progress)
{
    ui->setupUi(this);
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(closeTransfer()));
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(ok()));
}

Progress::~Progress()
{
    delete ui;
}

void Progress::setBarValue(int value) {
    ui->progressBar->setValue(value);
}

void Progress::setMaxValue(int max) {
    this->max = max;
    ui->progressBar->setMaximum(max);
}

void Progress::closeTransfer() {
    if(ui->progressBar->value() != max) {
        emit endTransfer();
    }
    else {
        this->close();
    }
}

void Progress::setLabel(QString label) {
    ui->label->setText(label);
}

void Progress::ok() {
    if(ui->progressBar->value() == max) {
        this->close();
    }
}


