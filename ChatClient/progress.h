#ifndef PROGRESS_H
#define PROGRESS_H

#include <QWidget>

namespace Ui {
class Progress;
}

class Progress : public QWidget
{
    Q_OBJECT
    
public:
    explicit Progress(QWidget *parent = 0);
    ~Progress();
    void setMaxValue(int);
    
private:
    Ui::Progress *ui;
    int max;

public slots:
    void setBarValue(int);
    void closeTransfer();
    void setLabel(QString label);
    void ok();
signals:
    void endTransfer();
};

#endif // PROGRESS_H
