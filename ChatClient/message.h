#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>
#include <QTcpSocket>
#include "messagestruct.h"

class Message : public QObject
{
    Q_OBJECT
public:
    explicit Message(QObject *parent = 0);
    Message(Message&);
    Message(int, int, int, int, char*);
    Message(int, int, int, int, QString);
    Message(int, int, int, int, QString fileName, int dataSize, QByteArray);

    int getMessageType();
    int getDest();
    int getFrom();
    int getFileSize();
    int getIsLast();
    QString getFileName();
    void setDest();
    void setType();
    void createMessage(char*, int);
    void createMessage(QByteArray, int);
    char* getBuffer();
    MessageStruct getStruct();



private:


    MessageStruct msg;
    
signals:
    
public slots:
    void send(QTcpSocket*);
    
};

#endif // MESSAGE_H
