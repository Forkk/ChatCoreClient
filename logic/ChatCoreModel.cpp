#include "ChatCoreModel.h"

ChatCoreModel::ChatCoreModel(QObject *parent) : QAbstractListModel(parent)
{
}

QVariant ChatCoreModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if (index.row() > count())
		return QVariant();

	const ChatCorePtr version = at(index.row());

	switch (role)
	{
	case Qt::DisplayRole:
        return version->name;

	case Qt::ToolTipRole:
        return version->address + ":" + QString::number(version->port);

	default:
		return QVariant();
	}
}

QVariant ChatCoreModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(section)
	Q_UNUSED(orientation)

	switch (role)
	{
	case Qt::DisplayRole:
		return "Name";

	case Qt::ToolTipRole:
		return "Address";

	default:
		return QVariant();
	}
}

int ChatCoreModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

void ChatCoreModel::init()
{
	ChatCorePtr core(new ChatCore());
	core->address = "localhost";
	core->name = "Local (Testing)";
	core->port = 1337;
	m_list.append(core);
}
