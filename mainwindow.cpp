#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include "aboutdialog.h"
#include "document.h"
#include "finddialog.h"
#include "modelmenu.h"
#include "replacedialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _findDock(nullptr)
    , _findDialog(nullptr)
    , _replaceDock(nullptr)
    , _replaceDialog(nullptr)
    , _document(new Document())
{
    ui->setupUi(this);

    _recentFilesMenu = new ModelMenu(tr("&Open Recent"), this);
    ui->menuFile->insertMenu(ui->menuFile->actions()[2], _recentFilesMenu);
    _recentFilesMenu->setModel(&_recentFilesModel);

    setCentralWidget(ui->textEdit);
    setDockOptions(DockOption::AllowNestedDocks | DockOption::AllowTabbedDocks);
    updateRecentFilesModel();
    addCustomContextMenu();
    newFile();
}

MainWindow::~MainWindow() = default;

void MainWindow::on_actionNew_triggered()
{
    checksave();
    newFile();
}

void MainWindow::on_actionOpen_triggered()
{
    checksave();
    openFile();
}

void MainWindow::on_actionSave_triggered()
{
    saveFile();
}

void MainWindow::on_actionSave_As_triggered()
{
    saveFileAs();
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionFind_triggered()
{
    if (_findDock == nullptr)
    {
        _findDock.reset(new QDockWidget(tr("Find"), this));
        _findDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

        _findDialog.reset(new FindDialog(*ui->textEdit, this));
        _findDock->setWidget(_findDialog.get());

        connect(_findDock.get(), &QDockWidget::visibilityChanged, [this](bool visibility){
            if (!visibility)
                _findDialog->reset();
        });

        if (_replaceDock)
        {
            tabifyDockWidget(_replaceDock, _findDock.get());
            qApp->processEvents(); // Force QT to process events to tabify the widget and allow us to focus it directly
        }
        else
            addDockWidget(Qt::RightDockWidgetArea, _findDock.get());
    }
    else if (!_findDock->isVisible())
    {
        _findDock->setVisible(true);
    }

    _findDock->raise();
    _findDock->setFocus();

    // Update the search string with the current selection, if exist
    const auto cursor = ui->textEdit->textCursor();
    const auto selectedText = cursor.selectedText();
    if (!selectedText.isEmpty())
        _findDialog->setText(selectedText);

    _findDialog->setFocus();
}

void MainWindow::on_actionReplace_triggered()
{
    if (_replaceDock == nullptr)
    {
        _replaceDock = new QDockWidget(tr("Replace"), this);
        _replaceDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

        _replaceDialog.reset(new ReplaceDialog(this));
        _replaceDock->setWidget(_replaceDialog.get());

        connect(_replaceDock, &QDockWidget::visibilityChanged, [this](bool visibility){
            //if (!visibility)
            //    _replaceDialog->reset();
        });


        if (_findDock)
        {
            tabifyDockWidget(_findDock.get(), _replaceDock);
            qApp->processEvents(); // Force QT to process events to tabify the widget and allow us to focus it directly
        }
        else
            addDockWidget(Qt::RightDockWidgetArea, _replaceDock);
    }
    else if (!_replaceDock->isVisible())
    {
        _replaceDock->setVisible(true);
    }

    _replaceDock->raise();
    _replaceDock->setFocus();

    // Update the search string with the current selection, if exist
    const auto cursor = ui->textEdit->textCursor();
    const auto selectedText = cursor.selectedText();
    //if (!selectedText.isEmpty())
    //    _replaceDialog->setText(selectedText);

    _replaceDialog->setFocus();
}

void MainWindow::on_actionSelect_All_triggered()
{
    ui->textEdit->selectAll();
}

void MainWindow::on_actionBold_triggered()
{
    auto font = ui->textEdit->currentFont();
    font.setBold(!font.bold());
    ui->textEdit->setCurrentFont(font);
}

void MainWindow::on_actionItalic_triggered()
{
    auto font = ui->textEdit->currentFont();
    font.setItalic(!font.italic());
    ui->textEdit->setCurrentFont(font);
}

void MainWindow::on_actionUnderline_triggered()
{
    auto font = ui->textEdit->currentFont();
    font.setUnderline(!font.underline());
    ui->textEdit->setCurrentFont(font);
}

void MainWindow::on_actionStrike_triggered()
{
    auto font = ui->textEdit->currentFont();
    font.setStrikeOut(!font.strikeOut());
    ui->textEdit->setCurrentFont(font);
}

void MainWindow::on_actionColor_triggered()
{
    const auto currentColor = ui->textEdit->currentCharFormat().foreground().color();
    const auto color = QColorDialog::getColor(currentColor, this, "Select a color");

    ui->textEdit->setTextColor(color);
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    const auto font = QFontDialog::getFont(&ok, ui->textEdit->currentFont(), this, "Select a font");
    if (ok)
        ui->textEdit->setCurrentFont(font);
}

void MainWindow::on_actionAbout_triggered()
{
    const QScopedPointer dlg(new AboutDialog(this));
    dlg->exec();
}

void MainWindow::on_actionHelp_triggered()
{
    QDesktopServices::openUrl(QUrl(QApplication::organizationDomain()));
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_textEdit_textChanged()
{
    _document->modified();
    updateCaption();
}

void MainWindow::on_actionPrint_triggered()
{
    const auto document = ui->textEdit->document();
    QPrinter printer;
    const QScopedPointer dlg(new QPrintDialog(&printer, this));
    if (dlg->exec() != QDialog::Accepted)
        return;
    document->print(&printer);
}

void MainWindow::openRecent()
{
    const auto action = qobject_cast<QAction *>(sender());
    if (action)
        openFile(action->data().toString());
}

void MainWindow::updateCaption()
{
    this->setWindowTitle(_document->fileName() + (_document->isSaved() ? "" : "*") + " - " + QApplication::applicationDisplayName());
}

void MainWindow::newFile()
{
    ui->textEdit->clear();
    ui->statusbar->showMessage("New File");

    _document.reset(new Document());

    updateCaption();
}

void MainWindow::openFile()
{
    const auto path = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(), _document->getSupportedExtensionFilter());
    openFile(path);
}

void MainWindow::openFile(const QString& path)
{
    if (path.isEmpty())
        return;

    if (!_document->setFullPath(path))
    {
        QMessageBox::critical(this, "Error", "The file format is not supported.");
        return;
    }

    if (!_document->load(*ui->textEdit))
    {
        QMessageBox::critical(this, "Error", "Can't load the file.");
        return;
    }

    updateCaption();

    QSettings settings;
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();
    recentFilePaths.removeAll(path);
    recentFilePaths.prepend(path);
    settings.setValue("recentFiles", recentFilePaths);

    updateRecentFilesModel();
}

void MainWindow::saveFile()
{
    const auto path = _document->fullPath();
    if (path.isEmpty())
    {
        saveFileAs();
        return;
    }

    if (!_document->save(*ui->textEdit))
    {
        QMessageBox::critical(this, "Error", "Can't save the file.");
        return;
    }

    updateCaption();

    QSettings settings;
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();
    recentFilePaths.removeAll(path);
    recentFilePaths.prepend(path);
    settings.setValue("recentFiles", recentFilePaths);

    updateRecentFilesModel();
}

void MainWindow::saveFileAs()
{
    const auto path = QFileDialog::getSaveFileName(this, "Save File", QDir::homePath(), _document->getSupportedExtensionFilter());
    if (path.isEmpty())
        return;
    _document->setFullPath(path);

    saveFile();
}

void MainWindow::checksave()
{
    if (_document->isSaved()) return;

    const auto value = QMessageBox::question(this, "Save File?", "You have un-saved changes would you like to save them?");
    if (value != QMessageBox::StandardButton::Yes) return;

    if (_document->fullPath().isEmpty())
        saveFileAs();
    else
        saveFile();
}

void MainWindow::addCustomContextMenu()
{
    // Custom context menu for textEdit
    const auto contextMenu = ui->textEdit->createStandardContextMenu();

    // Add actions to the context menu
    contextMenu->addSeparator();
    contextMenu->addAction(ui->actionBold);
    contextMenu->addAction(ui->actionItalic);
    contextMenu->addAction(ui->actionUnderline);
    contextMenu->addAction(ui->actionStrike);

    // Associate the menu with the QTextEdit
    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textEdit, &QTextEdit::customContextMenuRequested, this, [this, contextMenu](const QPoint &pos) {
        contextMenu->exec(ui->textEdit->mapToGlobal(pos));
    });
}

void MainWindow::updateRecentFilesModel()
{
    QSettings settings;
    const auto recentFilePaths = settings.value("recentFiles").toStringList();
    _recentFilesModel.setStringList(recentFilePaths);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    checksave();
    event->accept();
}
