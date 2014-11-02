#include <QtCore/qstatemachine.h>
#include "JsonTransport.h"
#include "logic/QCCCJson.h"

void JsonTransport::rawLine(QString raw) {
    QJsonObject obj = QCCCJson::ensureObject(raw);
    BufferLinePtr line(new BufferLine);
    line->network = QCCCJson::ensureString(obj["network"], "network");
    line->buffer = QCCCJson::ensureString(obj["buffer"], "buffer");
    line->data = obj.toVariantMap();
    emit lineArrived(line);
}

void JsonTransport::send(BufferLinePtr line) {
    QJsonObject object = QJsonObject::fromVariantMap(line->data);
    QJsonDocument doc(object);
    emit rawOutgoing(doc.toJson(QJsonDocument::Compact));
}

 "JsonTransport.moc"
