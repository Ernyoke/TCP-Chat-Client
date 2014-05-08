#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //ui->graphicsView->hide();
    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(loginButtonPressed()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(exit()));
}

Login::~Login()
{
    //delete socket;
    delete ui;
}

void Login::loginButtonPressed() {
    QString ip = ui->lineEdit->text();
    int port = ui->lineEdit_2->text().toInt();
    QString name = ui->lineEdit_3->text();
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    socket->connectToHost(ip, port);
    if(!socket->waitForConnected(3000)) {

    }
    else {
        int csize = 0;
        Message m(1, 0, 0, 0, ui->lineEdit_3->text().toUtf8());
        m.send(socket);
        socket->waitForReadyRead(-1);
        QByteArray chunk = socket->read(4);
        memcpy(&csize, chunk, 4);
        QByteArray buffer = socket->read(csize);
        Message message;
        message.createMessage(buffer, csize);
        int userID = message.getDest();
        if(strcmp(message.getBuffer(), "OK") == 0) {
            main =  new MainWindow(ip, port, name, socket, userID);
            connect(main, SIGNAL(exit()), this, SLOT(exitClient()));
            this->hide();
            disconnect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
            main->show();
        }
        else {
            QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ChatClient",
                                                                        "This username is already used by another user!",
                                                                        QMessageBox::Retry | QMessageBox::Close);
            if (resBtn == QMessageBox::Cancel) {
                socket->close();
                this->close();
            }
            else {
                socket->close();
                delete socket;
            }
        }
    }
    //this->show();
}

void Login::connectionError(QAbstractSocket::SocketError error) {
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ChatClient",
                                                                socket->errorString(),
                                                                QMessageBox::Retry | QMessageBox::Close);
    if(resBtn == QMessageBox::Close) {
        this->close();
    }

}

void Login::exitClient() {
    //this->show();
    //connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(connectionError(QAbstractSocket::SocketError)));
    //this->show();
    //disconnect(main, SIGNAL(exit()), this, SLOT(exitClient()));
//    delete main;
//    this->show();
}

void Login::exit() {
    this->close();
}
