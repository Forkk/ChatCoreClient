#pragma once

#include <QAbstractListModel>
#include "Utils.h"
#include "ChatCoreTransport.h"
#include "QCCC.h"
#include <QMap>
#include <QPair>

DEF_PTR(BufferContentModel)

class BufferContentModel : public QAbstractListModel {
Q_OBJECT
public:
    explicit BufferContentModel(QObject *parent = 0);

    BufferLinePtr at(int index) {
        return hasBuf ? getBuf()->lines[index] : BufferLinePtr();
    }

    const BufferLinePtr at(int index) const {
        return hasBuf ? getBuf()->lines[index] : BufferLinePtr();
    }

    int count() const {
        return hasBuf ? getBuf()->lines.count() : 0;
    }

    virtual QVariant data(const QModelIndex &index, int role) const;

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    virtual int rowCount(const QModelIndex &parent) const {
        return count();
    }

    virtual int columnCount(const QModelIndex &parent) const;

    QString const &getBuffer() const {
        return buffer;
    }

    QString const &getNetwork() const {
        return network;
    }

public slots:

    void init(BufferPtr buffer);

    void newData(BufferLinePtr line);

private:
    QString buffer;
    QString network;

    BufferPtr getBuf() {
        return QCCC->networks[network]->buffers[buffer];
    }

    BufferPtr const getBuf() const {
        return QCCC->networks[network]->buffers[buffer];
    }
    bool hasBuf = false;
};
