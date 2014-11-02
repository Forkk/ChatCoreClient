#pragma once

#include <exception>
#include "QCCC.h"

class QCCCError : public std::exception
{
public:
	QCCCError(QString) noexcept;
	virtual ~QCCCError() noexcept;

private:
	QString m_message;
};