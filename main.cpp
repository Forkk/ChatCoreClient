#include "QCCC.h"
#include "gui/MainWindow.h"

int main_gui(QChatCoreClient &app)
{
    // show main window
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}

int main(int argc, char *argv[])
{
    // initialize Qt
    QChatCoreClient app(argc, argv);

    Q_INIT_RESOURCE(qccc);

    switch (app.status())
    {
    case QChatCoreClient::Initialized:
        return main_gui(app);
    case QChatCoreClient::Failed:
        return 1;
    case QChatCoreClient::Succeeded:
        return 0;
    }
}
