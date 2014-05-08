#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "mainwindow.h"
#include <QDebug>
#include <QByteArray>
#include <QTcpServer>
#include <QGraphicsPixmapItem>

namespace Ui {
class Login;
}


class Login : public QDialog
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    
private:
    Ui::Login *ui;
    QTcpSocket* socket;
    MainWindow* main;

private slots:
    void loginButtonPressed();
    void connectionError(QAbstractSocket::SocketError);
    void exitClient();
    void exit();

signals:
    void sendInput(QString*, int, QString*);


};

#endif // LOGIN_H
