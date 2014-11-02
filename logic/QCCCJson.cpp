#include "QCCCJson.h"

#include <QString>
#include <QUrl>
#include <QStringList>
#include <math.h>

QJsonDocument QCCCJson::parseDocument(const QByteArray &data, const QString &what)
{
	QJsonParseError error;
	QJsonDocument doc = QJsonDocument::fromJson(data, &error);
	if (error.error != QJsonParseError::NoError)
	{
		throw JSONValidationError(what + " is not valid JSON: " + error.errorString() + " at " + error.offset);
	}
	return doc;
}

bool QCCCJson::ensureBoolean(const QJsonValue val, const QString what)
{
	if (!val.isBool())
		throw JSONValidationError(what + " is not boolean");
	return val.toBool();
}

QJsonValue QCCCJson::ensureExists(QJsonValue val, const QString what)
{
	if(val.isUndefined() || val.isUndefined())
		throw JSONValidationError(what + " does not exist");
	return val;
}

QJsonArray QCCCJson::ensureArray(const QJsonValue val, const QString what)
{
	if (!val.isArray())
		throw JSONValidationError(what + " is not an array");
	return val.toArray();
}

QJsonArray QCCCJson::ensureArray(const QJsonDocument &val, const QString &what)
{
	if (!val.isArray())
	{
		throw JSONValidationError(what + " is not an array");
	}
	return val.array();
}

double QCCCJson::ensureDouble(const QJsonValue val, const QString what)
{
	if (!val.isDouble())
		throw JSONValidationError(what + " is not a number");
	return val.toDouble();
}

int QCCCJson::ensureInteger(const QJsonValue val, const QString what)
{
	double ret = ensureDouble(val, what);
	if (fmod(ret, 1) != 0)
		throw JSONValidationError(what + " is not an integer");
	return ret;
}

QJsonObject QCCCJson::ensureObject(const QJsonValue val, const QString what)
{
	if (!val.isObject())
		throw JSONValidationError(what + " is not an object");
	return val.toObject();
}

QJsonObject QCCCJson::ensureObject(const QJsonDocument val, const QString what)
{
	if (!val.isObject())
		throw JSONValidationError(what + " is not an object");
	return val.object();
}

QString QCCCJson::ensureString(const QJsonValue val, const QString what)
{
	if (!val.isString())
		throw JSONValidationError(what + " is not a string");
	return val.toString();
}

QUrl QCCCJson::ensureUrl(const QJsonValue &val, const QString &what)
{
	const QUrl url = QUrl(ensureString(val, what));
	if (!url.isValid())
	{
		throw JSONValidationError(what + " is not an url");
	}
	return url;
}

int QCCCJson::ensureInteger(const QJsonValue val, QString what, const int def)
{
	if (val.isUndefined())
		return def;
	return ensureInteger(val, what);
}

void QCCCJson::writeString(QJsonObject &to, QString key, QString value)
{
	if (!value.isEmpty())
	{
		to.insert(key, value);
	}
}

void QCCCJson::writeStringList(QJsonObject &to, QString key, QStringList values)
{
	if (!values.isEmpty())
	{
		QJsonArray array;
		for(auto value: values)
		{
			array.append(value);
		}
		to.insert(key, array);
	}
}

QStringList QCCCJson::ensureStringList(const QJsonValue val, QString what)
{
	const QJsonArray array = ensureArray(val, what);
	QStringList out;
	for (const auto value : array)
	{
		out.append(ensureString(value));
	}
	return out;
}
