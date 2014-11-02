#include "QCCC.h"
#include <QLibraryInfo>
#include <QDir>
#include "pathutils.h"
#include "cmdutils.h"
#include <iostream>
#include <logic/JsonTransport.h>
#include "BuildConfig.h"

using namespace Util::Commandline;

QChatCoreClient::QChatCoreClient(int &argc, char **argv, bool root_override)
        : QApplication(argc, argv) {
    QHash<QString, QVariant> args;
    {
        Parser parser(FlagStyle::GNU, ArgumentStyle::SpaceAndEquals);

        // --help
        parser.addSwitch("help");
        parser.addShortOpt("help", 'h');
        parser.addDocumentation("help", "display this help and exit.");
        // --version
        parser.addSwitch("version");
        parser.addShortOpt("version", 'V');
        parser.addDocumentation("version", "display program version and exit.");
        // --dir
        parser.addOption("dir", applicationDirPath());
        parser.addShortOpt("dir", 'd');
        parser.addDocumentation("dir", "use the supplied directory as QCCC root instead of "
                "the binary location (use '.' for current)");
        // WARNING: disabled until further notice
        /*
        // --launch
        parser.addOption("launch");
        parser.addShortOpt("launch", 'l');
        parser.addDocumentation("launch", "tries to launch the given instance", "<inst>");
*/

        // parse the arguments
        try {
            args = parser.parse(arguments());
        }
        catch (ParsingError e) {
            std::cerr << "CommandLineError: " << e.what() << std::endl;
            std::cerr << "Try '%1 -h' to get help on QCCC's command line parameters."
                    << std::endl;
            m_status = QChatCoreClient::Failed;
            return;
        }

        // display help and exit
        if (args["help"].toBool()) {
            std::cout << qPrintable(parser.compileHelp(arguments()[0]));
            m_status = QChatCoreClient::Succeeded;
            return;
        }

        // display version and exit
        if (args["version"].toBool()) {
            std::cout << "Version " << BuildConfig.VERSION_STR.toStdString() << std::endl;
            std::cout << "Git " << BuildConfig.GIT_COMMIT.toStdString() << std::endl;
            m_status = QChatCoreClient::Succeeded;
            return;
        }
    }
    origcwdPath = QDir::currentPath();
    binPath = applicationDirPath();
    // change directory
    QString dirParam = args["dir"].toString();
    if (!dirParam.isEmpty()) {
        // the dir param. it makes QCCC data path point to whatever the user specified
        // on command line
        dataPath = dirParam;
    }
    else {
        dataPath = applicationDirPath();
    }

    if (!ensureFolderPathExists(dataPath) || !QDir::setCurrent(dataPath)) {
        // BAD STUFF. WHAT DO?
        m_status = QChatCoreClient::Failed;
        return;
    }

    if (root_override) {
        rootPath = binPath;
    }
    else {
#ifdef Q_OS_LINUX
        QDir foo(PathCombine(binPath, ".."));
        rootPath = foo.absolutePath();
#elif defined(Q_OS_WIN32)
		rootPath = binPath;
#elif defined(Q_OS_MAC)
		QDir foo(PathCombine(binPath, "../.."));
		rootPath = foo.absolutePath();
#endif
    }

// static data paths... mostly just for translations
#ifdef Q_OS_LINUX
    QDir foo(PathCombine(binPath, ".."));
    staticDataPath = foo.absolutePath();
#elif defined(Q_OS_WIN32)
	staticDataPath = binPath;
#elif defined(Q_OS_MAC)
	QDir foo(PathCombine(rootPath, "Contents/Resources"));
	staticDataPath = foo.absolutePath();
#endif

    initTranslations();

    m_status = QChatCoreClient::Initialized;
}

QChatCoreClient::~QChatCoreClient() {
    if (m_qccc_translator) {
        removeTranslator(m_qccc_translator.get());
    }
    if (m_qt_translator) {
        removeTranslator(m_qt_translator.get());
    }
}

void QChatCoreClient::initTranslations() {
    m_qt_translator.reset(new QTranslator());
    QLocale locale = QLocale::system();
    if (m_qt_translator->load("qt_" + locale.bcp47Name(),
            QLibraryInfo::location(QLibraryInfo::TranslationsPath))) {
        if (!installTranslator(m_qt_translator.get())) {
            m_qt_translator.reset();
        }
    }
    else {
        m_qt_translator.reset();
    }

    m_qccc_translator.reset(new QTranslator());
    if (m_qccc_translator->load("qccc_" + locale.bcp47Name(),
            QCCC->staticData() + "/translations")) {
        if (!installTranslator(m_qccc_translator.get())) {
            m_qccc_translator.reset();
        }
    }
    else {
        m_qccc_translator.reset();
    }
}

void QChatCoreClient::connectToCore(ChatCorePtr core, QString username, QString password, BufferContentModelPtr content, BufferModelPtr buffers) {
    m_connection.reset(new ChatCoreConnection(core, username, password, this));
    m_transport.reset(new JsonTransport());
    m_connection->setTransport(m_transport);
    connect(m_transport.get(), SIGNAL(lineArrived(BufferLinePtr)), content.get(), SLOT(newData(BufferLinePtr)), Qt::DirectConnection);
    connect(content.get(), SIGNAL(newBuf(Buffer * )), buffers.get(), SLOT(add(Buffer * )), Qt::DirectConnection);
    connect(m_connection.get(), SIGNAL(status(QString)), this, SIGNAL(connectionStatus(QString)));
    connect(m_transport.get(), SIGNAL(rawOutgoing(QString)), m_connection.get(), SLOT(send(QString)), Qt::DirectConnection);
    m_connection->start();
}
