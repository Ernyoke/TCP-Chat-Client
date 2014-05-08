#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <QDialog>

namespace Ui {
class ErrorMessage;
}

class ErrorMessage : public QDialog
{
    Q_OBJECT
    
public:
    explicit ErrorMessage(QString text, QWidget *parent = 0);
    ~ErrorMessage();
    
private:
    Ui::ErrorMessage *ui;

private slots:
    void okPressed();

signals:
    void destr();

};

#endif // ERRORMESSAGE_H
