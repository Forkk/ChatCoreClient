#pragma once

#include <QDialog>

namespace Ui {
    class JoinDialog;
}

class JoinDialog : public QDialog {
Q_OBJECT

public:
    explicit JoinDialog(QWidget *parent = 0);

    ~JoinDialog();

    QString channel() const {
        return m_channel;
    }

    QString network() const {
        return m_network;
    }

private slots:

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::JoinDialog *ui;
    QString m_channel;
    QString m_network;
};
