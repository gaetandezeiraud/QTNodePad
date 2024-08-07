#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QTextEdit>
#include <QDialog>
#include <QPushButton>
#include "searchhighlight.h"

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent, QTextEdit *textEdit);
    ~FindDialog();

    bool wholeWords() const;
    bool caseSensitive() const;
    bool backwards() const;

    void setText(const QString &newText);
    QString text() const;

    void reset();
    void setFocus();

private slots:
    void goPrevious();
    void goNext();
    void on_lineEdit_textChanged(const QString &text);
    void on_checkBox_CaseSensitive_checkStateChanged(const Qt::CheckState &state);
    void on_checkBox_MatchWholeWords_checkStateChanged(const Qt::CheckState &state);

    void on_lineEdit_returnPressed();

private:
    Ui::FindDialog *ui;
    QTextEdit *_textEdit;
    std::unique_ptr<SearchHighLight> _searchHighLight;

    QString _text;
    bool _wholeWords;
    bool _caseSensitive;
    bool _backwards;

    void init();
    void save();

    void findInTextEdit();
};

#endif // FINDDIALOG_H
