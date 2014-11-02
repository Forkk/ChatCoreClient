#include "ChatCoreConnection.h"
#include "ChatCoreTransport.h"

ChatCoreConnection::ChatCoreConnection(ChatCorePtr core, QString username,
                                       QString password, QObject *parent)
    : QThread(parent), m_core(core), m_user(username), m_password(password) {}

void ChatCoreConnection::run() {
  m_socket = std::shared_ptr<QTcpSocket>(new QTcpSocket());
  m_stream = std::shared_ptr<QTextStream>(new QTextStream(m_socket.get()));
  m_socket->connectToHost(m_core->address, m_core->port);
  m_socket->waitForConnected();
  while(!m_stream->atEnd()) {
    QString line = m_stream->readLine();
    if(m_transport) {
      m_transport->rawLine(line);
    }
  }
}
