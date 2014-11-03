#pragma once

#include <QAbstractItemModel>
#include <QString>
#include <QMap>
#include <Utils.h>
#include <QCCC.h>

DEF_PTR(BufferModel)

class BufferModel : public QAbstractItemModel {
Q_OBJECT

public:
    virtual ~BufferModel();

    NetworkPtr forName(QString name);

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    virtual QModelIndex parent(const QModelIndex &child) const;

    virtual int rowCount(const QModelIndex &index = QModelIndex()) const;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override {
        if (!index.isValid())
            return 0;

        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

public slots:

    void add(QString network, QString buffer);

private:
    int currentNetworkIndex;
};