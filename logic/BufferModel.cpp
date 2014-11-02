#include "BufferModel.h"
#include <QTreeView>
#include <QModelIndex>
#include <QtWidgets/qtablewidget.h>

BufferModel::~BufferModel() {
    for (Buffer *buf : m_data)
        delete buf;
}

int BufferModel::columnCount(const QModelIndex& parent) const {
    return 1;
}

QVariant BufferModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole) {
        return QVariant();
    }
}

QModelIndex BufferModel::index(int row, int column, const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    if(!parent.isValid()) {
        return createIndex(row, column, getNetwork(row));
    }
}

QModelIndex BufferModel::parent(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }

    Buffer *buf = static_cast<Buffer *>(index.internalPointer());
    if (buf != nullptr) {
        Network *n = buf->network;
        return createIndex(n->row, 0, n);
    }
    return QModelIndex();
}

int BufferModel::rowCount(const QModelIndex & parent) const {
    Network *item;
    if (parent.column() > 0)
        return 0;
    if (!parent.isValid()) {
        return currentNetworkIndex;
    }
    item = static_cast<Network*>(parent.internalPointer());
    if(item != nullptr) {
        return item->nBufs;
    }
    return 0;
}

Network *BufferModel::getNetwork(int row) const {
    return nullptr;
}

#include "BufferModel.moc"
