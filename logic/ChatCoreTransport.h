#pragma once

#include <QString>
#include <QVariantMap>
#include "Utils.h"

DEF_PTR_STRUCT(BufferLine)

struct BufferLine
{
	QString network;
	QString buffer;
	QVariantMap data;
};

DEF_PTR(ChatCoreTransport)

class ChatCoreTransport : public QObject
{
	Q_OBJECT

signals:

    void lineArrived(BufferLinePtr line);
	void rawOutgoing(QString data);

public slots:
	virtual void rawLine(QString line) = 0;

    virtual void send(BufferLinePtr line) = 0;
private:
};
