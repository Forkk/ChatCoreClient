#pragma once

#include <QAbstractItemModel>
#include <QString>
#include <QMap>
#include <Utils.h>

class Network;

class Buffer {
public:
    virtual ~Buffer() {
    };
    Network *network;
    QString name;
    int row = -1;
};

class Network : public Buffer {
public:
    int nBufs;
};

DEF_PTR(BufferModel)

class BufferModel : public QAbstractItemModel {
Q_OBJECT

public:
    virtual ~BufferModel();

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex parent(const QModelIndex &child) const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override {
        if (!index.isValid())
            return 0;

        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

public slots:

    void add(Buffer *buffer);

private:
    QList<Buffer *> m_data;
    int currentNetworkIndex;

    Network *getNetwork(int row) const;

    Buffer *getBuffer(Network *pNetwork, int row) const;
};