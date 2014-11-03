#include "BufferModel.h"
#include <QTreeView>

BufferModel::~BufferModel() {

}

NetworkPtr BufferModel::forName(QString name) {
    return QCCC->networks[name];
}

int BufferModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

QVariant BufferModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }
    if (role != Qt::DisplayRole) {
        return QVariant();
    }
    BaseBufferPtr ptr = BaseBufferPtr(static_cast<BaseBuffer *>(index.internalPointer()));
    if (ptr) {
        return ptr->name;
    }
    return QVariant();
}

QModelIndex BufferModel::index(int row, int column, const QModelIndex &parent) const {
    if (!parent.isValid()) {
        return createIndex(row, column, QCCC->networks[QCCC->networks.keys()[row]].get());
    }
    BaseBufferPtr ptr = BaseBufferPtr(static_cast<BaseBuffer *>(parent.internalPointer()));
    NetworkPtr networkPtr = std::dynamic_pointer_cast<Network>(ptr);
    if (networkPtr) {
        return createIndex(row, column, networkPtr->buffers[networkPtr->buffers.keys()[row]].get());
    }
    return QModelIndex();
}

QModelIndex BufferModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }

    BaseBufferPtr ptr = BaseBufferPtr(static_cast<BaseBuffer *>(index.internalPointer()));
    NetworkPtr networkPtr = std::dynamic_pointer_cast<Network>(ptr);
    if (networkPtr) {
        return QModelIndex();
    }
    BufferPtr bufferPtr = std::dynamic_pointer_cast<Buffer>(ptr);
    if (!bufferPtr) {
        return QModelIndex();
    }
    networkPtr = bufferPtr->network;
    return createIndex(QCCC->networks.keys().indexOf(networkPtr->name, 0), 0, networkPtr.get());
}

int BufferModel::rowCount(const QModelIndex &index) const {
    if (!index.isValid()) {
        return QCCC->networks.count();
    }
    NetworkPtr ptr = QCCC->networks[QCCC->networks.keys()[index.row()]];
    if (ptr) {
        return ptr->buffers.count();
    }
    return 0;
}

void BufferModel::add(QString network, QString buffer) {
    beginResetModel();
    if (!QCCC->networks.contains(network)) {
        QCCC->networks[network] = NetworkPtr(new Network);
        QCCC->networks[network]->name = network;
    }
    NetworkPtr net = QCCC->networks[network];
    if (!net->buffers.contains(buffer)) {
        net->buffers[buffer] = BufferPtr(new Buffer);
        net->buffers[buffer]->name = buffer;
    }
    endResetModel();
}

#include "BufferModel.moc"
