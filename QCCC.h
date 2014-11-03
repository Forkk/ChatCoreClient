#pragma once

#include <QApplication>
#include <QTranslator>
#include <memory>
#include "Utils.h"

#ifdef QCCC
#undef QCCC
#endif
#define QCCC (static_cast<QChatCoreClient *>(QCoreApplication::instance()))

DEF_PTR(BufferLine)
DEF_PTR(Network)
DEF_PTR(BaseBuffer)
DEF_PTR(Buffer)
DEF_PTR_STRUCT(ChatCore)
DEF_PTR(BufferContentModel)
DEF_PTR(BufferModel)
DEF_PTR(ChatCoreConnection)
DEF_PTR(ChatCoreTransport)

class BaseBuffer {
public:
    QString name;
protected:
    virtual void hack() {
    };
};

class BufferLine {
public:
    BufferPtr buffer;
    QVariantMap data;
};

class Buffer : public BaseBuffer {
public:
    QList<BufferLinePtr> lines;
    NetworkPtr network;
};


class Network : public BaseBuffer {
public:
    QString name;
    QMap<QString, BufferPtr> buffers;
};

class QChatCoreClient : public QApplication {
Q_OBJECT


public:
    enum Status {
        Failed,
        Succeeded,
        Initialized
    };

    QChatCoreClient(int &argc, char **argv, bool root_override = false);


    virtual ~QChatCoreClient();

    Status status() {
        return m_status;
    }

    /// this is the static data. it stores things that don't move.
    const QString &staticData() {
        return staticDataPath;
    }

    /// this is the root of the 'installation'. Used for automatic updates
    const QString &root() {
        return rootPath;
    }

    /// this is the where the binary files reside
    const QString &bin() {
        return binPath;
    }

    /// this is the work/data path. All user data is here.
    const QString &data() {
        return dataPath;
    }

    /**
    * this is the original work path before it was changed by the adjustment mechanism
    */
    const QString &origcwd() {
        return origcwdPath;
    }

    const ChatCoreConnectionPtr &getConnection() const {
        return m_connection;
    }

    const ChatCoreTransportPtr &getTransport() const {
        return m_transport;
    }

    QMap<QString, NetworkPtr> networks;

signals:

    void connectionStatus(QString error);

public slots:

    void connectToCore(ChatCorePtr core, QString username, QString password, BufferContentModelPtr model, BufferModelPtr ptr);

private:
    Status m_status = QChatCoreClient::Failed;
    std::shared_ptr<QTranslator> m_qt_translator;
    std::shared_ptr<QTranslator> m_qccc_translator;
    ChatCoreTransportPtr m_transport;
    ChatCoreConnectionPtr m_connection;

    void initTranslations();

    QString rootPath;
    QString staticDataPath;
    QString binPath;
    QString dataPath;
    QString origcwdPath;
};
