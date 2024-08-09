#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    ui->label_Name->setText(QApplication::applicationDisplayName());
    ui->label_Version->setText(QApplication::applicationVersion());
    ui->label_Company->setText(QApplication::organizationName());
    ui->label_Url->setText(QApplication::organizationDomain());
}

AboutDialog::~AboutDialog() = default;

void AboutDialog::on_buttonBox_accepted()
{
    accept();
}

