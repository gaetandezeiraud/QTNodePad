#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindDialog)
{
    ui->setupUi(this);
    init();
    save();
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::on_buttonBox_accepted()
{
    save();
    accept();
}


void FindDialog::on_buttonBox_rejected()
{
    reject();
}

void FindDialog::goBack()
{
    save();
    _backwards = true;
    accept();
}

QString FindDialog::text() const
{
    return _text;
}

bool FindDialog::wholeWords() const
{
    return _wholeWords;
}

bool FindDialog::caseSensitive() const
{
    return _caseSensitive;
}

bool FindDialog::backwards() const
{
    return _backwards;
}

void FindDialog::init()
{
    QPushButton* btnBack = new QPushButton("Back", this);
    connect(btnBack, &QPushButton::clicked, this, &FindDialog::goBack);
    ui->buttonBox->addButton(btnBack, QDialogButtonBox::ActionRole);
}

void FindDialog::save()
{
    _text = ui->lineEdit->text();
    _caseSensitive = ui->checkBox_CaseSensitive->isChecked();
    _wholeWords = ui->checkBox_MatchWholeWords->isChecked();
    _backwards = false;
}
