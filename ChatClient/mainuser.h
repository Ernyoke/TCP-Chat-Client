#ifndef MAINUSER_H
#define MAINUSER_H

#include <QWidget>
#include <QDir>
#include <QFileDialog>

namespace Ui {
class MainUser;
}

class MainUser : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainUser(QWidget *parent = 0);
    ~MainUser();
    void setServerIp(QString);
    void setUserName(QString);
    void setPort(int);
    void setUserId(int);
    QString &getServerIp();
    QString &getUserName();
    int getPort();
    int getUserId();
    QString getFilePath();
    
private:
    Ui::MainUser *ui;
    QString userName;
    QString serverIp;
    int port;
    int userID;
    QString fileLocation;

private slots:
    void setFilePath();
    void applyChanges();
};

#endif // MAINUSER_H
