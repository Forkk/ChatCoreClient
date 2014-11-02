#include "JsonTransport.h"
#include "logic/QCCCJson.h"

void JsonTransport::rawLine(QString raw) {
  QJsonObject obj = QCCCJson::ensureObject(raw);
  BufferLine line;
  line.network = QCCCJson::ensureString(obj["network"], "network");
  line.buffer = QCCCJson::ensureString(obj["buffer"], "buffer");
  line.data = obj.toVariantMap();
  emit lineArrived(line);
}
void JsonTransport::send(BufferLine line) {
  
}

#include "JsonTransport.moc"
