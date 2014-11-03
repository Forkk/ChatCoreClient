#pragma once

#include <QString>
#include <QVariantMap>
#include "Utils.h"
#include "BufferModel.h"


DEF_PTR(ChatCoreTransport)

class ChatCoreTransport : public QObject {
Q_OBJECT

public:
    ChatCoreTransport(BufferModelPtr model) : m_model(model) {
    }

signals:

    void lineArrived(BufferLinePtr line);

public slots:

    virtual void rawLine(QString line) = 0;

    virtual void send(BufferLinePtr line) = 0;

protected:
    BufferModelPtr m_model;
};
