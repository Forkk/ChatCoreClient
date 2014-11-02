#pragma once

#include <QMainWindow>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void on_send_clicked();

	void on_message_returnPressed();

	void on_actionPart_Channel_triggered();

	void on_actionConnect_To_Core_triggered();

	void on_actionJoin_Channel_triggered();

private:
	Ui::MainWindow *ui;
	void send(QString text);
};
