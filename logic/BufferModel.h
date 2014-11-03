#pragma once

#include <QAbstractItemModel>
#include <QString>
#include <QMap>
#include <Utils.h>
#include <QCCC.h>
#include <QtGui/qstandarditemmodel.h>

DEF_PTR(BufferModel)

class BufferModel : public QStandardItemModel {
Q_OBJECT

public:

public slots:

    void add(QString network, QString buffer);

};