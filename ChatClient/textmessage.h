#ifndef TEXTMESSAGE_H
#define TEXTMESSAGE_H

#include <QObject>
#include "message.h"

class TextMessage : public Message
{
    Q_OBJECT
public:
    explicit TextMessage(QObject *parent = 0);
    
signals:
    
public slots:
    
};

#endif // TEXTMESSAGE_H
