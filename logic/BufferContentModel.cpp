#include <kdebug.h>
#include "BufferContentModel.h"

BufferContentModel::BufferContentModel(QObject *parent) : QAbstractListModel(parent) {
}

QVariant BufferContentModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() > count())
        return QVariant();

    const BufferLinePtr version = at(index.row());

    switch (role) {
        case Qt::DisplayRole:
            return version->data;

        default:
            return QVariant();
    }
}

QVariant BufferContentModel::headerData(int section, Qt::Orientation orientation, int role) const {
    Q_UNUSED(section)
    Q_UNUSED(orientation)

    switch (role) {
        case Qt::DisplayRole:
            return "Content";

        default:
            return QVariant();
    }
}

int BufferContentModel::columnCount(const QModelIndex &parent) const {
    return 1;
}

void BufferContentModel::init(BufferPtr buffer) {
    beginResetModel();

    this->buffer = buffer->name;
    this->network = buffer->network->name;

    endResetModel();
}

void BufferContentModel::newData(BufferLinePtr line) {
    beginInsertRows(QModelIndex(), count(), count());

    line->buffer->lines.append(line);

    endInsertRows();
}
