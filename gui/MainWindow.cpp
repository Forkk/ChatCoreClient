#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "CoreConnectDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_send_clicked()
{
	send(ui->message->text());
}

void MainWindow::on_message_returnPressed()
{
	send(ui->message->text());
}

void MainWindow::send(QString text)
{
}

void MainWindow::on_actionPart_Channel_triggered()
{
}

void MainWindow::on_actionConnect_To_Core_triggered()
{
    CoreConnectDialog dialog(this);
    if(!dialog.exec()) {
        ChatCorePtr core = dialog.core();
        QString username = dialog.username();
        QString password = dialog.password();

    }

}

void MainWindow::on_actionJoin_Channel_triggered()
{
}
