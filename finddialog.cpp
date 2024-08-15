#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QTextEdit &textEdit, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FindDialog)
    , _textEdit(&textEdit)
    , _searchHighLight(new SearchHighLight(_textEdit->document()))
{
    ui->setupUi(this);
    init();
    save();
    setFocus();
}

FindDialog::~FindDialog() = default;

void FindDialog::reset()
{
    _searchHighLight->searchText("");
}

void FindDialog::setFocus()
{
    ui->lineEdit->setFocus();
}

void FindDialog::goPrevious()
{
    save();
    _backwards = true;
    findInTextEdit();
}

void FindDialog::goNext()
{
    save();
    findInTextEdit();
}

void FindDialog::setText(const QString &newText)
{
    _text = newText;
    ui->lineEdit->setText(_text);
    _searchHighLight->searchText(_text);
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
    const auto btnPrevious = new QPushButton("Previous", this);
    const auto btnNext = new QPushButton("Next", this);

    connect(btnPrevious, &QPushButton::clicked, this, &FindDialog::goPrevious);
    ui->buttonBox->addButton(btnPrevious, QDialogButtonBox::ActionRole);

    connect(btnNext, &QPushButton::clicked, this, &FindDialog::goNext);
    ui->buttonBox->addButton(btnNext, QDialogButtonBox::ActionRole);
}

void FindDialog::save()
{
    _text = ui->lineEdit->text();
    _caseSensitive = ui->checkBox_CaseSensitive->isChecked();
    _wholeWords = ui->checkBox_MatchWholeWords->isChecked();
    _backwards = false;
}

void FindDialog::findInTextEdit()
{
    QTextDocument::FindFlags flags;
    if (caseSensitive())
        flags |= QTextDocument::FindFlag::FindCaseSensitively;
    if (wholeWords())
        flags |= QTextDocument::FindFlag::FindWholeWords;
    if (backwards())
        flags |= QTextDocument::FindFlag::FindBackward;

    _textEdit->find(text(), flags);
}

void FindDialog::on_lineEdit_textChanged(const QString &text)
{
    _text = text;
    _searchHighLight->searchText(_text);
}

void FindDialog::on_checkBox_CaseSensitive_checkStateChanged(const Qt::CheckState &state)
{
    _searchHighLight->setCaseSensitive(state == Qt::Checked);
    _searchHighLight->searchText(_text);
}

void FindDialog::on_checkBox_MatchWholeWords_checkStateChanged(const Qt::CheckState &state)
{
    _searchHighLight->setWholeWord(state == Qt::Checked);
    _searchHighLight->searchText(_text);
}

void FindDialog::on_lineEdit_returnPressed()
{
    goNext();
}

