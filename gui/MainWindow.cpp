#include <QCCC.h>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CoreConnectDialog.h"
#include "JoinDialog.h"
#include "logic/ChatCoreTransport.h"
#include "logic/BufferContentModel.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow), m_bufferModel(new BufferModel), m_contentModel(new BufferContentModel) {
    ui->setupUi(this);
    ui->bufferList->setModel(m_bufferModel.get());
    ui->buffer->setModel(m_contentModel.get());
    connect(QCCC, SIGNAL(connectionStatus(QString)), this, SLOT(connectionStatus(QString)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_send_clicked() {
    send(ui->message->text());
}

void MainWindow::on_message_returnPressed() {
    send(ui->message->text());
}

void MainWindow::send(QString text) {
    BufferLinePtr line(new BufferLine);
    NetworkPtr net = QCCC->networks[m_contentModel->getNetwork()];
    line->buffer = net->buffers[m_contentModel->getBuffer()];
    line->data = QVariantMap();
    line->data.insert("command", "send-msg");
    line->data.insert("network", m_contentModel->getNetwork());
    line->data.insert("dest", m_contentModel->getBuffer());
    line->data.insert("message", text);

    QCCC->getTransport()->send(line);
}

void MainWindow::on_actionPart_Channel_triggered() {
}

void MainWindow::on_actionConnect_To_Core_triggered() {
    CoreConnectDialog dialog(this);
    if (dialog.exec()) {
        ChatCorePtr core = dialog.core();
        QString username = dialog.username();
        QString password = dialog.password();
        m_contentModel.reset(new BufferContentModel());
        m_bufferModel.reset(new BufferModel());

        QCCC->connectToCore(core, username, password, m_contentModel, m_bufferModel);
    }

}

void MainWindow::on_actionJoin_Channel_triggered() {
    JoinDialog dialog(this);
    if (dialog.exec()) {
        m_bufferModel->add(dialog.network(), dialog.channel());

        BufferLinePtr line(new BufferLine);
        NetworkPtr net = QCCC->networks[dialog.network()];
        line->buffer = net->buffers[dialog.channel()];
        line->data = QVariantMap();
        line->data.insert("command", "join-chan");
        line->data.insert("network", dialog.network());
        line->data.insert("channel", dialog.channel());

        QCCC->getTransport()->send(line);
    }
}

void MainWindow::connectionStatus(QString string) {
    ui->statusbar->showMessage(string);
}

void MainWindow::on_bufferList_clicked(const QModelIndex &index)
{
    QStandardItem* item = m_bufferModel->itemFromIndex(index);
    if(item->parent() == m_bufferModel->invisibleRootItem()) {
        NetworkPtr ptr = QCCC->networks[item->text()];
        m_contentModel->init(ptr->buffers["*"]);
    } else {
        QStandardItem* network = item->parent();
        NetworkPtr net = QCCC->networks[network->text()];
        BufferPtr buf = net->buffers[item->text()];
        m_contentModel->init(buf);
    }
}
