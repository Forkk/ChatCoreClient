#include <QtCore/qstatemachine.h>
#include "JsonTransport.h"
#include "logic/QCCCJson.h"
#include "ChatCoreConnection.h"

void JsonTransport::rawLine(QString raw) {
    QJsonObject obj = QCCCJson::ensureObject(raw);
    BufferLinePtr line(new BufferLine);
    m_model->add(QCCCJson::ensureString(obj["network"], "network"), QCCCJson::ensureString(obj["buffer"], "buffer"));
    NetworkPtr net = QCCC->networks[QCCCJson::ensureString(obj["network"], "network")];
    line->buffer = net->buffers[QCCCJson::ensureString(obj["buffer"], "buffer")];
    line->data = obj.toVariantMap();
    emit lineArrived(line);
}

void JsonTransport::send(BufferLinePtr line) {
    QJsonObject object = QJsonObject::fromVariantMap(line->data);
    QJsonDocument doc(object);
    QCCC->getConnection()->send(doc.toJson(QJsonDocument::Compact));
}
