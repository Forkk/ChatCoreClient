#pragma once

#include <QThread>
#include <QTcpSocket>
#include <QTextStream>
#include "ChatCoreModel.h"
#include "ChatCoreTransport.h"

DEF_PTR(ChatCoreConnection)

class ChatCoreConnection : public QThread
{
    Q_OBJECT
public:
    explicit ChatCoreConnection(ChatCorePtr core, QString username, QString password, QObject *parent = 0);

	virtual void run();
signals:

    void status(QString error);
public slots:
	void setTransport(ChatCoreTransportPtr transport) {
		m_transport = transport;
	}

    void send(QString rawMsg);
private:
	std::shared_ptr<QTcpSocket> m_socket;
	std::shared_ptr<QTextStream> m_stream;
	ChatCorePtr m_core;
	QString m_user;
	QString m_password;
	ChatCoreTransportPtr m_transport;
};
