#include "chatroominterface.h"
#include "ui_chatroom.h"

ChatRoomInterFace::ChatRoomInterFace(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatRoom)
{
    ui->setupUi(this);
}

ChatRoomInterFace::~ChatRoomInterFace()
{
    delete ui;
}
