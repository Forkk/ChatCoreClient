#include "CoreConnectDialog.h"
#include "ui_CoreConnectDialog.h"

CoreConnectDialog::CoreConnectDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::CoreConnectDialog), m_model(new ChatCoreModel(this))
{
	ui->setupUi(this);

    ui->coreSelect->setModel(m_model.get());

    m_model->init();
}

CoreConnectDialog::~CoreConnectDialog()
{
	delete ui;
}

void CoreConnectDialog::on_buttonBox_accepted()
{
	m_username = ui->username->text();
	m_password = ui->password->text();
    m_core = m_model->at(ui->coreSelect->currentIndex().row());
    QDialog::accept();
}

void CoreConnectDialog::on_buttonBox_rejected() {
    QDialog::reject();
}
