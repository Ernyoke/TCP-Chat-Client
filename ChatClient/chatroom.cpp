#include "chatroom.h"
#include "ui_chatroom.h"

chatRoom::chatRoom(QWidget *parent) :
    QWidget(parent), ui(new Ui::ChatRoom)
{
    ui->setupUi(this);
}

chatRoom::~chatRoom() {
    //delete ui;
}

chatRoom::chatRoom(QTcpSocket *socket, QMap<int, User*>* global, QPlainTextEdit *display, QTextEdit *input, QPushButton *send, MainUser *mainUser) {
    this->global = global;
    this->display = display;
    this->input = input;
    this->sendButton = send;
    this->socket = socket;
    this->mainUser = mainUser;
    this->roomID = 0;
    this->userlist = NULL;
    this->sendButton->installEventFilter(this);
    this->input->installEventFilter(this);
    userNr = 0;
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendText()));
}

chatRoom::chatRoom(int rId, QTcpSocket *socket, QTabWidget *tab, MainUser* mainUser, QMap<int, User*>* global) : ui(new Ui::ChatRoom)  {
    ui->setupUi(this);
    this->tab = tab;
    this->roomID = rId;
    this->tabID = tab->count();
    //asd
    //ChatRoomInterFace *inf = new ChatRoomInterFace;
    this->mainUser = mainUser;
    QString roomName = intToChar(rId);
    tab->addTab(this, "Private(" + roomName + ")");
    this->display = ui->plainTextEdit;
    this->input = ui->textEdit;
    this->sendButton = ui->pushButton;
    this->sendFile = ui->pushButton_4;
    this->leaveRoom = ui->pushButton_3;
    this->add = ui->pushButton_2;
    this->userlist = ui->listWidget;
    this->socket = socket;
    this->uname = ui->lineEdit;
    this->sendButton->installEventFilter(this);
    this->input->installEventFilter(this);
    connect(sendButton, SIGNAL(clicked()), this, SLOT(sendText()));
    connect(leaveRoom, SIGNAL(clicked()), this, SLOT(leave()));
    connect(add, SIGNAL(clicked()), this, SLOT(inviteUser()));
    connect(sendFile, SIGNAL(clicked()), this, SLOT(sendFileSlot()));
    this->global = global;
    userNr = 0;
    //if()
}

void chatRoom::sendText() {
    QString text = input->toPlainText();
    if(text != "" ) {
    input->setText("");
    display->appendPlainText(mainUser->getUserName() + ": " + text);
    Message m(5, mainUser->getUserId(), this->roomID, 1, text);
    m.send(socket);
    }
}

void chatRoom::displayText(Message *m, int index) {
    QMap<int, User*>::iterator it;
    it = global->find(m->getFrom());
    QString username;
    User *user = it.value();
    if(it == global->end()) {
        username.append("not yet connected");
    }
    else {
        username.append(user->getUsername());
    }
    qDebug() << m->getFrom();
    QString mText(m->getBuffer());
    if(roomID != 0) {
        QTabBar *tabBar = tab->tabBar();
        if(tabID != index) {
            tabBar->setTabTextColor(m->getDest(), Qt::red);
        }
    }
    //display->setStyleSheet("font: 9pt \"Courier\";");
    display->appendPlainText(username + ": " + mText);
}

void chatRoom::addUser(int userId, User *name) {
    users.insert(userId, name);
    if(this->userlist != 0) {
        ui->listWidget->addItem(name->getUsername());
    }
    display->appendPlainText(name->getUsername() + " joined!");
    userNr++;
}

bool chatRoom::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if(keyEvent->key() == Qt::Key_Return) {
                sendButton->click();
                return true;
            }
            else {
                return QObject::eventFilter(obj, event);
            }
        } else {
            // standard event processing
            return QObject::eventFilter(obj, event);
        }
}

 void chatRoom::leave() {
     qDebug() << "leave";
     emit leaveSignal(tabID, roomID);
 }

 void chatRoom::deleteUser(int userID) {
     QString username;
     QMap<int, User*>::Iterator it = users.find(userID);
     User* user = it.value();
     username = user->getUsername();
     users.erase(it);
     int n = userlist->count();
     for (int i=0; i < n; i++) {
          QListWidgetItem* item = userlist->item(i);
          if(item->text().compare(username) == 0) {
              userlist->takeItem(i);
              break;
          }
     }
     userNr--;

 }

 void chatRoom::inviteUser() {
     QString username = ui->lineEdit->text();
     bool found = false;
     int userId = 0;
     for(QMap<int, User*>::Iterator it = global->begin(); it != global->end(); ++it) {
         User* user = it.value();
         if(user->getUsername().compare(username) == 0) {
             //addUser(user->getId(), user);
             found = true;
             userId = user->getId();
         }
     }
     if(!found) {
         QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ChatClient",
                                                                     tr("User not found\n"),
                                                                     QMessageBox::Ok | QMessageBox::Cancel);
         if (resBtn == QMessageBox::Ok || resBtn == QMessageBox::Cancel) {
             ui->lineEdit->setText("");
         }
     }
     else {
         QMap<int, User*>::Iterator it = users.find(userId);
         if(it != users.end()) {
             QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ChatClient",
                                                                         tr("User already joined!\n"),
                                                                         QMessageBox::Ok | QMessageBox::Cancel);
             if (resBtn == QMessageBox::Ok || resBtn == QMessageBox::Cancel) {
                 ui->lineEdit->setText("");
             }
         }
         else {
             Message m(6, mainUser->getUserId(), roomID, 1, intToChar(userId));
             m.send(socket);
         }
     }
 }

 char* chatRoom::intToChar(int value) {
     char* res = (char*)malloc(sizeof(char));
         int i = 0;
         if(value == 0) {
             res[0] = '0';
         }
         while(value > 0) {
             if(i >= 1) {
                 res = (char*)realloc(res, (i + 1) * sizeof(char));
             }
             res[i] = value % 10 + 48;
             value /= 10;
             ++i;
         }
         res[i] = '\0';
         return res;
 }

 void chatRoom::sendFileSlot() {
     if(userNr == 2) {
         QFileDialog explore;
         fileLocation = explore.getOpenFileName(this);
         if(fileLocation != NULL) {
            file.setFileName(fileLocation);
             file.open(QIODevice::ReadOnly);
             int n = fileLocation.length();
             int i = n - 1;
             while(fileLocation[i] != '/') {
                 --i;
             }
             ++i;
             fileName = fileLocation.mid(i);
             //qDebug() << fileName << " " << file.size();
             progBar = new Progress();
             connect(progBar, SIGNAL(endTransfer()), this, SLOT());
             csize = 0;
             progBar->setMaxValue(file.size());
             progBar->setBarValue(0);
             progBar->setLabel("Waiting for accept...");
             progBar->show();
             isLast = 0;
             sendingData = true;
             /*if(!isActive) {
                 Message m(5, mainUser->getUserId(), this->roomID, 1, "wake up");
                 m.send(socket);
                 isActive = true;
             }*/
             Message m(9, mainUser->getUserId(), roomID, 0, fileName, file.size(), "");
             m.send(socket);
         }
     }
         else {
             QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ChatClient",
                                                                         "Too many users in this chatroom!",
                                                                         QMessageBox::Ok | QMessageBox::Cancel);
     }
 }

 void chatRoom::AcceptFileTransfer(Message *message) {
     QMap<int, User*>::iterator it;
     it = global->find(message->getFrom());
     User *user = it.value();

     QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ChatClient",
                                                                 user->getUsername() + " sent you a file: " + message->getFileName(),
                                                                 QMessageBox::Ok | QMessageBox::Cancel);
     if (resBtn == QMessageBox::Ok) {
         fileName = message->getFileName();
         fileLocation = mainUser->getFilePath() + "/" + message->getFileName();
         qDebug() << "File location:" << fileLocation;
         fileSize = message->getFileSize();
         file.setFileName(fileLocation);
         file.open(QIODevice::Append);
         Message m(10, mainUser->getUserId(), roomID, 0, message->getFileName(), fileSize, "");
         m.send(socket);
         progBar = new Progress();
         connect(progBar, SIGNAL(endTransfer()), this, SLOT());
         progBar->setMaxValue(fileSize);
         progBar->setBarValue(0);
         progBar->setLabel("Transfering...");
         progBar->show();
         csize = 0;
         isLast = 0;
         receivingData = true;
     }
     else {
         fileLocation = message->getFileName();
         fileSize = message->getFileSize();
         Message m(11, mainUser->getUserId(), roomID, 0, fileName, fileSize, "");
         m.send(socket);
         receivingData = false;
     }

 }

 void chatRoom::transferFile() {
     QByteArray barray;
     if(csize == 0) {
         progBar->setLabel("Transfering...");
     }
     //progBar->setBarValue(0);
     barray = file.read(8000);
     qDebug() << "transfered";
         if(barray.size() < 8000) {
             isLast = 1;
             file.close();
             sendingData = false;
         }
         csize += barray.size();
         progBar->setBarValue(csize);
         Message m(12, mainUser->getUserId(), roomID, isLast, fileName, barray.size(), barray);
         qDebug() << sizeof(m) ;
         m.send(socket);
         if(csize == file.size()) {
             progBar->setLabel("Transfer completed!");
         }
 }

 void chatRoom::receivingFile(Message *message) {
     csize += message->getFileSize();
     file.write(message->getBuffer(), message->getFileSize());
     if(message->getIsLast() == 1){
         file.close();
         progBar->setLabel("Transfer completed!");
         receivingData = false;
         progBar->setBarValue(csize);
     }
     else {
         //socket->flush();
         if(receivingData) {
            Message m(10, mainUser->getUserId(), roomID, 0, fileName, 0, "");
            m.send(socket);
         }
         else {
            Message m(11, mainUser->getUserId(), roomID, 0, fileName, 0, "");
            m.send(socket);
         }
         progBar->setBarValue(csize);
     }
 }

 void chatRoom::declineTransfer() {
     if(receivingData) {
         progBar->setLabel("Transfer closed!");
         receivingData = false;
     }
     if(sendingData) {
         progBar->setLabel("Transfer closed!");
         isLast = 1;
         sendingData = false;
     }
 }

 int chatRoom::getTabID() {
     return this->tabID;
 }

