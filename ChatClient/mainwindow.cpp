#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <Windows.h>

MainWindow::MainWindow(QString ip, int port, QString username, QTcpSocket* socket, int userID, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
     ui->setupUi(this);
     csize = -1;
     this->setWindowTitle("ChatClient - " + username);
     this->socket = socket;
    mainUser = new MainUser();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
    connect(ui->listWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(initiateNewChatRoom(QModelIndex)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabColor(int)));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    qDebug() << ip;
    mainUser->setServerIp(ip);
    mainUser->setPort(port);
    mainUser->setUserName(username);
    mainUser->setUserId(userID);
        this->show();
        Message m2(2,1,1,1, "");
        m2.send(socket);
        chatRoom* ch = new chatRoom(socket, &users, ui->plainTextEdit, ui->textEdit, ui->pushButton, mainUser);
        rooms.insert(0, ch);

}
MainWindow::~MainWindow()
{
    for(QMap<int, User*>::Iterator it = users.begin(); it != users.end(); ++it) {
        delete it.value();
    }

    for(QMap<int, chatRoom*>::Iterator it = rooms.begin(); it != rooms.end(); ++it) {
        delete it.value();
    }
    delete socket;
    delete mainUser;
    delete ui;
    qDebug() << "destruct";
}

void MainWindow::readBuffer() {
    qDebug() << "avfirst:" << socket->bytesAvailable();
    if(csize == -1) {
        QByteArray chunk = socket->read(4);
        memcpy(&csize, chunk, 4);
    }
    if(csize <= socket->bytesAvailable()) {
        QByteArray buffer = socket->read(csize);
        Message message;
        qDebug() << "memsize: " << csize << "av:" << socket->bytesAvailable();
        message.createMessage(buffer, csize);
        qDebug () << message.getMessageType();
        //qDebug() << message.getFileName();
        csize = -1;
        switch(message.getMessageType()) {
        case 2:
            //update main list
            updateList(message.getBuffer());
            break;
        case 3:
            //create new chatroom
            createNewChatRoom(message.getDest(), message.getFrom());
            break;
        case 4:
        {
            //get chatrooom information
            char *uId = new char[5];
            int cId;
            char* buffer = message.getBuffer();
            int n = strlen(buffer);
            int j = 0;
            for(int i = 0; i < n; ++i) {
                if(buffer[i] == ' ') {
                    uId[j] = '\0';
                    cId = charToInt(uId);
                    j = 0;
                    QMap<int, User*>::Iterator uit = users.find(cId);
                    QMap<int, chatRoom*>::Iterator cit = rooms.find(message.getDest());
                    if(cit == rooms.end()) {
                        createNewChatRoom(message.getDest(), message.getFrom());
                        break;
                    }
                    cit = rooms.find(message.getDest());
                    if(uit != users.end()) {
                        chatRoom *r = cit.value();
                        r->addUser(cId, uit.value());
                    }
                }
                else {
                    uId[j++] = buffer[i];
                }
            }
            break;
        }
        case 5:
        {
            //text message
            QMap<int, chatRoom*>::Iterator it;
            it = rooms.find(message.getDest());
            if(it == rooms.end()) {
                createNewChatRoom(message.getDest(), message.getFrom());
                it = rooms.find(message.getDest());
            }
            chatRoom *r = it.value();
            if(it != rooms.end()) {
                r->displayText(&message, ui->tabWidget->currentIndex());
            }
            break;
        }
        case 6:
        {
            //delete user from a chatroom (user leaves the room)
            QMap<int, chatRoom*>::Iterator it;
            it = rooms.find(message.getDest());
            chatRoom *x  = it.value();
            if(it != rooms.end()) {
                chatRoom *r = it.value();
                r->deleteUser(message.getFrom());
            }
            break;
        }
        case 8:
        {
            //an user logged out, delete him from main list
            QString username;
            QMap<int, User*>::Iterator it = users.find(message.getFrom());
            User* user = it.value();
            username = user->getUsername();
            users.erase(it);
            int n = ui->listWidget->count();
            for (int i=0; i < n; i++) {
                 QListWidgetItem* item = ui->listWidget->item(i);
                 if(item->text().compare(username) == 0) {
                     ui->listWidget->takeItem(i);
                     break;
                 }
            }
            delete user;
            break;
        }
        case 9:
        {
            QMap<int, chatRoom*>::Iterator it;
            it = rooms.find(message.getDest());
            if(it == rooms.end()) {
                createNewChatRoom(message.getDest(), message.getFrom());
                it = rooms.find(message.getDest());
                }
            chatRoom *r = it.value();
            if(it != rooms.end()) {
                r->AcceptFileTransfer(&message);
            }
            break;

        }
        case 10:
        {
            QMap<int, chatRoom*>::Iterator it;
            it = rooms.find(message.getDest());
            chatRoom *x  = it.value();
            if(it != rooms.end()) {
                chatRoom *r = it.value();
                r->transferFile();
            }
            break;
        }
        case 11:
        {
            QMap<int, chatRoom*>::Iterator it;
            it = rooms.find(message.getDest());
            chatRoom *x  = it.value();
            if(it != rooms.end()) {
                chatRoom *r = it.value();
                r->declineTransfer();
            }
            break;

        }
        case 12:
        {
            QMap<int, chatRoom*>::Iterator it;
            it = rooms.find(message.getDest());
            chatRoom *x  = it.value();
            if(it != rooms.end()) {
                chatRoom *r = it.value();
                r->receivingFile(&message);
            }
            break;
        }

        }
        if(socket->bytesAvailable() != 0) {
            readBuffer();
        }
    }
}

QByteArray MainWindow::intToByte(int value,  int size) {
    char* res = new char[size];
    int j = 0, result = 0;
    for(int i = size - 1; i >= 0; --i) {
        for(int j = 0; j < 8; ++j) {
            result = result + (1 << j) * ((value >> j) & 1);
        }
        char c = result;
        result = 0;
        res[i] = c;
        value = value >> 8;
    }
    QByteArray bytes;
    bytes.append(res);
    return bytes ;
}

int MainWindow::toInt(char* buffer) {
    int size = strlen(buffer);
    int value = 0;
    for(int i = size - 1; i >= 0; --i) {
        value = (buffer[i] - '0') + value * 10;
    }
    return value;
}

void MainWindow::updateList(char* buffer) {
    QMap<int, chatRoom*>::Iterator it;
    it = rooms.find(0);
    chatRoom *r = it.value();
    char nr[10];
    char name[20];
    int id, j = 0, k;
    int aux = 0;
    for(int i = 0; buffer[i] != '\0'; ++i) {
        if(buffer[i] == 1) {
            if(aux % 2 == 0) {
                nr[j] = '\0';
                id = toInt(nr);
            }
            else {
                name[j] = '\0';
                QString uname(name);
                User *user = new User(id, uname);
                r->addUser(id, user);
                users.insert(id, user);
                QListWidgetItem add(uname);
                add.setData(Qt::UserRole, QVariant(id));
                ui->listWidget->addItem(new QListWidgetItem(add));

            }
            j = 0;
            aux++;
        }
        else {
            if(aux % 2 == 0) {
                nr[j++] = buffer[i];
            }
            else {
                name[j++] = buffer[i];
            }
        }
    }
}

void MainWindow::initiateNewChatRoom(QModelIndex index) {
    int id = ui->listWidget->item(index.row())->data(Qt::UserRole).toUInt();
    if(id != mainUser->getUserId()) {
        QString addUser =  ui->listWidget->item(index.row())->data(Qt::UserRole).toString();
        Message m(3, mainUser->getUserId(), 0, 1, addUser);
        m.send(socket);
    }
    else {
        qDebug() << "own id";
    }
}

void MainWindow::createNewChatRoom(int rId, int creatorID) {
    chatRoom *room = new chatRoom(rId, socket, ui->tabWidget, mainUser, &users);
    rooms.insert(rId, room);
    Message m(4, 0, rId, 1,  "");
    m.send(socket);
    connect(room, SIGNAL(leaveSignal(int, int)), this, SLOT(leaveRoom(int, int)));
    if(creatorID == mainUser->getUserId()) {
        ui->tabWidget->setCurrentIndex(room->getTabID());
    }
}

int MainWindow::charToInt(char* buffer) {
    int n = strlen(buffer);
    int ans = 0;
    for(int i = n -1; i >= 0; --i) {
        ans = 10 * ans + buffer[i] - 48;
    }
    return ans;
}

void MainWindow::leaveRoom(int ID, int roomId) {
    int n = ui->tabWidget->count();
    ui->tabWidget->removeTab(ID);
    QMap<int, chatRoom*>::Iterator it;
    it = rooms.find(roomId);
    rooms.erase(it);
    Message m(7, mainUser->getUserId(), roomId, 1,  "");
    m.send(socket);
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ChatClient",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        event->accept();
        Message m(6, mainUser->getUserId(), 0, 1,  "");
        m.send(socket);
        //socket->disconnect();
        socket->close();
        //emit exit();
    }
}

void MainWindow::tabColor(int index) {
    QTabBar *tabBar = ui->tabWidget->tabBar();
    tabBar->setTabTextColor(index, Qt::black);
}


void MainWindow::error(QAbstractSocket::SocketError socketError) {
    QMessageBox::StandardButton resBtn = QMessageBox::question( this, "ChatClient",
                                                                socket->errorString(),
                                                                QMessageBox::Cancel | QMessageBox::Ok);
    if(resBtn == QMessageBox::Ok) {
        this->close();
    }
}

void MainWindow::on_actionLogout_2_triggered() {
    this->close();
}

void MainWindow::on_actionProperties_triggered() {
    mainUser->show();
}

