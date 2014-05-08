#include "mainuser.h"
#include "ui_mainuser.h"

MainUser::MainUser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainUser)
{
    ui->setupUi(this);
    this->fileLocation = QDir::currentPath();
    ui->lineEdit->setText(this->fileLocation);
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(setFilePath()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(applyChanges()));
}

MainUser::~MainUser()
{
    delete ui;
}

void MainUser::setServerIp(QString ip) {
    this->serverIp = ip;
    ui->label_6->setText(this->serverIp);
}

void MainUser::setPort(int port) {
    this->port = port;
    ui->label_7->setText(QString::number(this->port));
}

void MainUser::setUserName(QString name) {
    this->userName = name;
    ui->label_5->setText(this->userName);
}

QString& MainUser::getServerIp() {
    return serverIp;
}

QString& MainUser::getUserName() {
    return userName;
}

int MainUser::getPort() {
    return port;
}

void MainUser::setUserId(int id) {
    this->userID = id;
}

int MainUser::getUserId() {
    return this->userID;
}

void MainUser::setFilePath() {
    QFileDialog explore;
    QString path = explore.getExistingDirectory(this);
    ui->lineEdit->setText(path);
}

void MainUser::applyChanges() {
    fileLocation = ui->lineEdit->text();
    this->close();
}

QString MainUser::getFilePath() {
    return this->fileLocation;
}
