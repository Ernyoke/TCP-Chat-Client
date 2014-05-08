#ifndef CHATROOM_H
#define CHATROOM_H

#include <QObject>
#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>
#include <QProgressBar>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QTabBar>
#include <QLineEdit>
#include <QGridLayout>
#include <QListWidget>
#include <QTabWidget>
#include <QMessageBox>
#include "message.h"
#include "mainuser.h"
#include "user.h"
#include "chatroominterface.h"
#include "progress.h"

class chatRoom : public QWidget
{
    Q_OBJECT
public:
    explicit chatRoom(QWidget *parent = 0);
    ~chatRoom();
    chatRoom(QTcpSocket*, QMap<int, User*>*, QPlainTextEdit*, QTextEdit *, QPushButton*, MainUser*);
    chatRoom(int, QTcpSocket*, QTabWidget *tab, MainUser *mainUser, QMap<int, User *> *);

    void displayText(Message*, int);
    void addUser(int, User*);
    void deleteUser(int);
    void AcceptFileTransfer(Message*);
    void receivingFile(Message *message);
    void declineTransfer();
    void transferFile();
    int getTabID();


private:
    QPlainTextEdit* display;
    QTextEdit* input;
    QPushButton* sendButton;
    QPushButton* leaveRoom;
    QPushButton* sendFile;
    QPushButton* add;
    QLineEdit* uname;
    QListWidget* userlist;
    QTcpSocket* socket;
    MainUser* mainUser;
    int roomID;
    QMap<int, User*> users;
    QMap<int, User*>* global;
    int tabID;
    QTabWidget *tab;
    QString fileLocation;
    QString fileName;
    QFile file;
    int fileSize;
    Progress *progBar;
    int csize;
    int userNr;
    int isLast;
    bool receivingData;
    bool sendingData;

    char* intToChar(int);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void leaveSignal(int, int);
    
public slots:
    void sendText();
    void leave();
    void inviteUser();
    void sendFileSlot();

private:
    Ui::ChatRoom *ui;
    
};

#endif // CHATROOM_H
