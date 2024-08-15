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
#include <QStringListModel>

#define RECENT_FILES_MAX 5

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class Document;
class FindDialog;
class ModelMenu;
class ReplaceDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

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
    // Add features
    void addCustomContextMenu();

    // Recent files
    void updateRecentFilesModel();

    //
    void updateCaption();
    void newFile();
    void openFile();
    void openFile(const QString& path);
    void saveFile();
    void saveFileAs();
    void checksave();

private:
    QScopedPointer<Ui::MainWindow> ui;
    QScopedPointer<Document> _document;

    // Recent files
    ModelMenu* _recentFilesMenu = nullptr;
    QStringListModel _recentFilesModel;

    // Docking
    QScopedPointer<QDockWidget> _findDock;
    QScopedPointer<FindDialog> _findDialog;

    QDockWidget *_replaceDock = nullptr;
    QScopedPointer<ReplaceDialog> _replaceDialog;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
