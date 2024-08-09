#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class ReplaceDialog;
}

class ReplaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReplaceDialog(QWidget *parent = nullptr);
    ~ReplaceDialog() override;

    bool all() const;
    QString text() const;
    QString replaceText() const;

private slots:
    void on_buttonBox_rejected();
    void replace();
    void replaceAll();

private:
    QScopedPointer<Ui::ReplaceDialog> ui;

    bool _all = false;
    QString _text;
    QString _replaceText;

    void init();
    void save();
};

#endif // REPLACEDIALOG_H
