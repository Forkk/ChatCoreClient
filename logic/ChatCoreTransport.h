/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2014  Alex Sieberer <robotbrainify@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <QString>
#include <QVariantMap>
#include "Utils.h"

struct BufferLine
{
	QString network;
	QString buffer;
	QVariantMap data;
};

DEF_PTR(ChatCoreTransport)

class ChatCoreTransport : public QObject
{
	Q_OBJECT

signals:
	void lineArrived(BufferLine line);
	void rawOutgoing(QString data);

public slots:
	virtual void rawLine(QString line) = 0;
	virtual void send(BufferLine line) = 0;
private:
};
