#include "user.h"

User::User(QObject *parent) :
    QObject(parent)
{
}

User::User(int id, QString username) {
    this->id = id;
    this->username = username;
}

int User:: getId() {
    return this->id;
}

QString User::getUsername() {
    return username;
}
