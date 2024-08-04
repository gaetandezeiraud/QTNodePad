#include "replacedialog.h"
#include "ui_replacedialog.h"

ReplaceDialog::ReplaceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ReplaceDialog)
{
    ui->setupUi(this);
    init();
    save();
}

ReplaceDialog::~ReplaceDialog()
{
    delete ui;
}

void ReplaceDialog::on_buttonBox_rejected()
{
    reject();
}

void ReplaceDialog::replace()
{
    save();
    accept();
}

void ReplaceDialog::replaceAll()
{
    save();
    _all = true;
    accept();
}

void ReplaceDialog::init()
{
    QPushButton* btnReplace = new QPushButton("Replace", this);
    QPushButton* btnReplaceAll = new QPushButton("Replace All", this);

    ui->buttonBox->addButton(btnReplace, QDialogButtonBox::ButtonRole::ActionRole);
    ui->buttonBox->addButton(btnReplaceAll, QDialogButtonBox::ButtonRole::ActionRole);

    connect(btnReplace, &QPushButton::clicked, this, &ReplaceDialog::replace);
    connect(btnReplaceAll, &QPushButton::clicked, this, &ReplaceDialog::replaceAll);
}

void ReplaceDialog::save()
{
    _text = ui->lineEdit_Find->text();
    _replaceText = ui->lineEdit_Replace->text();
    _all = false;
}

QString ReplaceDialog::replaceText() const
{
    return _replaceText;
}

QString ReplaceDialog::text() const
{
    return _text;
}

bool ReplaceDialog::all() const
{
    return _all;
}

