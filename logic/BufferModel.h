#pragma once

#include <QAbstractItemModel>
#include <QString>
#include <QMap>

struct Network {
	QString name;
	int row;
	int nBufs;
};

struct Buffer {
	QString name;
	Network* network;
	int row;
};

class BufferModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	virtual ~BufferModel();
	
	virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	
private:
	QList<Buffer*> m_data;
	int currentNetworkIndex;

    Network *getNetwork(int row) const;
};
