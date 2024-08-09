#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AboutDialog(QWidget *parent = nullptr);
    ~AboutDialog() override;

private slots:
    void on_buttonBox_accepted();

private:
    QScopedPointer<Ui::AboutDialog> ui;
};

#endif // ABOUTDIALOG_H
