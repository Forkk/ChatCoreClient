#pragma once
#include <QApplication>
#include <QTranslator>
#include <memory>

#ifdef QCCC
#undef QCCC
#endif
#define QCCC (static_cast<QChatCoreClient *>(QCoreApplication::instance()))

class QChatCoreClient : public QApplication
{
	Q_OBJECT

public:
	enum Status
	{
		Failed,
		Succeeded,
		Initialized
	};

	QChatCoreClient(int &argc, char **argv, bool root_override = false);

	virtual ~QChatCoreClient();

	Status status()
	{
		return m_status;
	}

	/// this is the static data. it stores things that don't move.
	const QString &staticData()
	{
		return staticDataPath;
	}
	/// this is the root of the 'installation'. Used for automatic updates
	const QString &root()
	{
		return rootPath;
	}
	/// this is the where the binary files reside
	const QString &bin()
	{
		return binPath;
	}
	/// this is the work/data path. All user data is here.
	const QString &data()
	{
		return dataPath;
	}
	/**
	 * this is the original work path before it was changed by the adjustment mechanism
	 */
	const QString &origcwd()
	{
		return origcwdPath;
	}

private:
	Status m_status = QChatCoreClient::Failed;
	std::shared_ptr<QTranslator> m_qt_translator;
	std::shared_ptr<QTranslator> m_qccc_translator;
	void initTranslations();

	QString rootPath;
	QString staticDataPath;
	QString binPath;
	QString dataPath;
	QString origcwdPath;
};
