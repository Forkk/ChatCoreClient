#pragma once

#include "ChatCoreTransport.h"

class JsonTransport : public ChatCoreTransport {
Q_OBJECT
public slots:

    virtual void rawLine(QString line);

    virtual void send(BufferLinePtr line);

private:
};
