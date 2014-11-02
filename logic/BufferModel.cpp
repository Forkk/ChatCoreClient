#include "BufferModel.h"
#include <QTreeView>

BufferModel::~BufferModel() {
    QMap<QString, Network *> networks;
    for (Buffer *buf : m_data) {
        networks[buf->network->name] = buf->network;
        delete buf;
    }
    for (Network *network : networks.values()) {
        delete network;
    }
}

int BufferModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant BufferModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    Buffer *buf = static_cast<Buffer *>(index.internalPointer());
    Network *net = dynamic_cast<Network *>(buf);
    if (net) {
        return net->name;
    }
    if (buf) {
        return buf->name;
    }
    return QVariant();
}

QModelIndex BufferModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if (!parent.isValid()) {
        return createIndex(row, column, getNetwork(row));
    }

    Network *net = dynamic_cast<Network *>(static_cast<Buffer *>(parent.internalPointer()));
    if (net) {
        return createIndex(row, column, getBuffer(net, row));
    }
    return QModelIndex();
}

QModelIndex BufferModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }

    Buffer *buf = static_cast<Buffer *>(index.internalPointer());
    if (buf) {
        Network *n = buf->network;
        return createIndex(n->row, 0, n);
    }
    return QModelIndex();
}

int BufferModel::rowCount(const QModelIndex &parent) const {
    Network *item;
    if (parent.column() > 0)
        return 0;
    if (!parent.isValid()) {
        return currentNetworkIndex;
    }
    item = dynamic_cast<Network *>(static_cast<Buffer *>(parent.internalPointer()));
    if (item) {
        return item->nBufs;
    }
    return 0;
}

Network *BufferModel::getNetwork(int row) const {
    QMap<int, Network *> networks;
    for (Buffer *buf : m_data) {
        Network *net = buf->network;
        networks[net->row] = net;
    }
    return networks[row];
}

Buffer *BufferModel::getBuffer(Network *pNetwork, int row) const {
    QMap<int, Buffer *> buffers;
    for (Buffer *buf : m_data) {
        Network *net = buf->network;
        if (net->name == pNetwork->name) {
            buffers[buf->row] = buf;
        }
    }
    return buffers[row];
}

void BufferModel::add(Buffer *buffer) {
    beginResetModel();

    m_data.append(buffer);
    buffer->row = buffer->network->nBufs++;
    if (buffer->network->row != -1) {
        buffer->network->row = currentNetworkIndex++;
    }

    endResetModel();
}

 "BufferModel.moc"
