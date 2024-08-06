#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDockWidget>
#include <QPrinter>
#include <QPrintDialog>
#include <QList>
#include <QSettings>
#include "finddialog.h"
#include "replacedialog.h"
#include "aboutdialog.h"
#include "document.h"

#define RECENT_FILES_MAX 5

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionExit_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionPaste_triggered();
    void on_actionFind_triggered();
    void on_actionReplace_triggered();
    void on_actionSelect_All_triggered();
    void on_actionBold_triggered();
    void on_actionItalic_triggered();
    void on_actionUnderline_triggered();
    void on_actionStrike_triggered();
    void on_actionColor_triggered();
    void on_actionFont_triggered();
    void on_actionHelp_triggered();
    void on_actionAbout_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_textEdit_textChanged();
    void on_actionPrint_triggered();
    void openRecent();

private:
    Ui::MainWindow *ui;

    QMenu* _recentFilesMenu;
    QList<QAction*> _recentFileActionList;

    QDockWidget *_findDock;
    FindDialog *_findDialog;

    QDockWidget *_replaceDock;
    ReplaceDialog *_replaceDialog;

    Document *_document;

    void updateCaption();
    void newFile();
    void openFile();
    void openFile(const QString& path);
    void saveFile();
    void saveFileAs();
    void checksave();

    void adjustForCurrentFile(const QString& filePath);
    void updateRecentActionList();

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
