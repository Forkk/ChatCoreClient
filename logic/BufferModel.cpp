#include "BufferModel.h"
#include <QTreeView>

void BufferModel::add(QString network, QString buffer) {
    QStandardItem *item;
    if (!QCCC->networks.contains(network)) {
        appendRow(item = new QStandardItem(network));
        sort(0);
        QCCC->networks[network] = NetworkPtr(new Network);
        QCCC->networks[network]->name = network;
    } else {
        item = findItems(network)[0];
    }
    NetworkPtr net = QCCC->networks[network];
    if (!net->buffers.contains(buffer)) {
        item->appendRow(new QStandardItem(buffer));
        item->sortChildren(0);
        net->buffers[buffer] = BufferPtr(new Buffer);
        net->buffers[buffer]->name = buffer;
    }
}
