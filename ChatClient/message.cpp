#include "message.h"

Message::Message(QObject *parent) :
    QObject(parent)
{

}
Message::Message(Message& m) {
    msg = m.getStruct();
}


Message::Message(int type, int from, int dest, int isLast, char* buffer) {
    msg.type = type;
    msg.dest = dest;
    msg.from = from;
    strcpy(msg.data, buffer);
    msg.islast = isLast;
    msg.dataSize = 0;
}

Message::Message(int type, int from, int dest, int isLast, QString buffer) {
    msg.type = type;
    msg.dest = dest;
    msg.from = from;
    memcpy(msg.data, buffer.toUtf8(), buffer.size());
    msg.islast = isLast;
    msg.dataSize = 0;
    msg.data[buffer.size()] = '\0';
}

Message::Message(int type, int from, int dest, int isLast, QString fileName, int dataSize, QByteArray buffer) {
    msg.type = type;
    msg.dest = dest;
    msg.from = from;
    msg.islast = isLast;
    msg.dataSize = dataSize;
    memcpy(msg.fileName, fileName.toUtf8(), fileName.size());
    msg.fileName[fileName.size()] = '\0';
    memcpy(msg.data, buffer, buffer.size());
}


void Message::createMessage(char *buffer, int size){
    memcpy(&msg, buffer, size);
}

void Message::createMessage(QByteArray buffer, int size) {
    memcpy(&msg, buffer, size);
}

void Message::send(QTcpSocket* socket) {
    void* myPtr = &msg;
    int msize = sizeof(msg);
    qDebug() << "msgSize: " << msize;
    void* asd = &msize;
    char* ssize = (char*)asd;
    char *data = (char*)myPtr;
    char* sendD = new char[sizeof(int) + msize];
    memcpy(sendD, ssize, sizeof(int));
    memcpy(sendD + sizeof(int), data, msize);
    socket->write(sendD, sizeof(int) + msize);
}



char* Message::getBuffer() {
    return msg.data;
}

int Message::getMessageType() {
    return msg.type;
}

int Message::getDest() {
    return msg.dest;
}

int Message::getFrom() {
    return msg.from;
}

QString Message::getFileName() {
    QString name(msg.fileName);
    //qDebug() << fileName;
    return name;
}

int Message::getFileSize() {
    return msg.dataSize;
}

int Message::getIsLast() {
    return msg.islast;
}

MessageStruct Message::getStruct() {
    return msg;
}

