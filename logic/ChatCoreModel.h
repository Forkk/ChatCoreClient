#ifndef CHATCOREMODEL_H
#define CHATCOREMODEL_H

#include <QAbstractListModel>
#include "Utils.h"

DEF_PTR_STRUCT(ChatCore)

struct ChatCore
{
    QString name;
    QString address;
    int port;
};

DEF_PTR(ChatCoreModel)

class ChatCoreModel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit ChatCoreModel(QObject *parent = 0);

	ChatCorePtr at(int index)
	{
		return m_list[index];
	}

	const ChatCorePtr at(int index) const
	{
        if (index < 0 || index >= count()) {
            index = 0;
        }
		return m_list[index];
	}

	int count() const
	{
		return m_list.length();
	}

	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	virtual int rowCount(const QModelIndex &parent) const
	{
		return count();
	}
	virtual int columnCount(const QModelIndex &parent) const;

signals:

public slots:
    void init();

private:
	QList<ChatCorePtr> m_list;
};

#endif // CHATCOREMODEL_H
