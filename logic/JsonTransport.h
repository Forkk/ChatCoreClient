#pragma once

#include "ChatCoreTransport.h"

class JsonTransport : public ChatCoreTransport {
Q_OBJECT
public:
    JsonTransport(BufferModelPtr model) : ChatCoreTransport(model) {
    }
public slots:

    virtual void rawLine(QString line);

    virtual void send(BufferLinePtr line);

private:
};
