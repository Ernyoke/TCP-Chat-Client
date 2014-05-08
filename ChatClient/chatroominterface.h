#ifndef CHATROOMINTERFACE_H
#define CHATROOMINTERFACE_H

#include <QWidget>

namespace Ui {
class ChatRoom;
}

class ChatRoomInterFace : public QWidget
{
    Q_OBJECT
    
public:
    explicit ChatRoomInterFace(QWidget *parent = 0);
    ~ChatRoomInterFace();
    
private:
    Ui::ChatRoom *ui;
};

#endif // CHATROOM_H
