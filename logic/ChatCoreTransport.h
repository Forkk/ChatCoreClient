#pragma once

#include <QString>
#include <QVariantMap>
#include "Utils.h"

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
	void lineArrived(BufferLine line);
	void rawOutgoing(QString data);

public slots:
	virtual void rawLine(QString line) = 0;
	virtual void send(BufferLine line) = 0;
private:
};
