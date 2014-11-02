#include <QtCore/qstring.h>
#include "QCCCError.h"

QCCCError::QCCCError(QString message) noexcept : m_message(message)
{
}

QCCCError::~QCCCError() noexcept
{
}
