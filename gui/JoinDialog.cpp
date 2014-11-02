#include "JoinDialog.h"
#include "ui_JoinDialog.h"

JoinDialog::JoinDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::JoinDialog) {
    ui->setupUi(this);
}

JoinDialog::~JoinDialog() {
    delete ui;
}

void JoinDialog::on_buttonBox_accepted() {
    m_network = ui->network->text();
    m_channel = ui->channel->text();
    QDialog::accept();
}

void JoinDialog::on_buttonBox_rejected() {
    QDialog::reject();
}
