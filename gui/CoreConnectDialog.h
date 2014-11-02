#pragma once

#include <QDialog>
#include "logic/ChatCoreModel.h"

namespace Ui
{
class CoreConnectDialog;
}

class CoreConnectDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CoreConnectDialog(QWidget *parent = 0);
	~CoreConnectDialog();
    QString username() {
        return m_username;
    }
    QString password() {
        return m_password;
    }
    ChatCorePtr core() {
        return m_core;
    }

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
	Ui::CoreConnectDialog *ui;
    QString m_username;
    QString m_password;
    ChatCorePtr m_core;
    ChatCoreModelPtr m_model;
};
