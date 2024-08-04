#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindDialog(QWidget *parent = nullptr);
    ~FindDialog();

    bool wholeWords() const;
    bool caseSensitive() const;
    bool backwards() const;

    QString text() const;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void goBack();

private:
    Ui::FindDialog *ui;

    QString _text;
    bool _wholeWords;
    bool _caseSensitive;
    bool _backwards;

    void init();
    void save();
};

#endif // FINDDIALOG_H
