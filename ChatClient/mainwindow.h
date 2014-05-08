#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDataStream>
#include <QMap>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QTabBar>
#include <QMessageBox>
#include "chatroom.h"
#include "chatroominterface.h"
#include "mainuser.h"
#include "messagestruct.h"
#include "message.h"
#include "user.h"
#include "errormessage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QString, int, QString, QTcpSocket*, int, QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    //Login* login;
    MainUser* mainUser;
    QTcpSocket* socket;
    int csize;
    QByteArray intToByte(int, int);
    QMap<int, User*> users;
    QMap<int, chatRoom*> rooms;


    void updateList(char*);
    void createNewChatRoom(int, int);
    int toInt(char*);
    int charToInt(char*);
    void closeEvent(QCloseEvent*);

private slots:
    void readBuffer();
    void initiateNewChatRoom(QModelIndex);
    void leaveRoom(int, int);
    void tabColor(int);
    void error(QAbstractSocket::SocketError);
    void on_actionLogout_2_triggered();
    void on_actionProperties_triggered();

signals:
    void exit();
};

#endif // MAINWINDOW_H
