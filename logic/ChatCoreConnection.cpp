#include "ChatCoreConnection.h"
#include <iostream>

ChatCoreConnection::ChatCoreConnection(ChatCorePtr core, QString username,
        QString password, QObject *parent)
        : QThread(parent), m_core(core), m_user(username), m_password(password) {
}

void ChatCoreConnection::run() {
    m_socket = std::shared_ptr<QTcpSocket>(new QTcpSocket());
    m_stream = std::shared_ptr<QTextStream>(new QTextStream(m_socket.get()));
    m_socket->connectToHost(m_core->address, m_core->port);
    emit status("Connecting to core...");
    if (m_socket->waitForConnected()) {
        emit status("Connected!");
        while (!m_stream->atEnd()) {
            QString line = m_stream->readLine();
            if (m_transport) {
                m_transport->rawLine(line);
            }
        }
    } else {
        emit status("Timeout!");
    }
}

void ChatCoreConnection::send(QString rawMsg) {
    std::cout << rawMsg.toStdString();
    *m_stream.get() << rawMsg << "\n";
}
