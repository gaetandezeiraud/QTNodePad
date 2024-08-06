#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _findDock(nullptr)
    , _findDialog(nullptr)
    , _replaceDock(nullptr)
    , _replaceDialog(nullptr)
    , _path("")
    , _changed(false)
{
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
    setDockOptions(DockOption::AllowNestedDocks | DockOption::AllowTabbedDocks);

    // Recent files
    QAction* recentFileAction = 0;
    for(auto i = 0; i < RECENT_FILES_MAX; ++i){
        recentFileAction = new QAction(this);
        recentFileAction->setVisible(false);
        connect(recentFileAction, &QAction::triggered, this, &MainWindow::openRecent);
        _recentFileActionList.append(recentFileAction);
    }

    _recentFilesMenu = new QMenu(tr("&Open Recent"));
    ui->menuFile->insertMenu(ui->menuFile->actions()[2], _recentFilesMenu);

    //_recentFilesMenu = ui->menuFile->addMenu(tr("Open Recent"));

    // ui->menuFile->actions()[3]

    for(auto i = 0; i < RECENT_FILES_MAX; ++i)
        _recentFilesMenu->addAction(_recentFileActionList.at(i));

    updateRecentActionList();

    // Custom context menu for textEdit
    QMenu *contextMenu = ui->textEdit->createStandardContextMenu();

    // Add actions to the context menu
    contextMenu->addSeparator();
    contextMenu->addAction(ui->actionBold);
    contextMenu->addAction(ui->actionItalic);
    contextMenu->addAction(ui->actionUnderline);
    contextMenu->addAction(ui->actionStrike);

    // Associate the menu with the QTextEdit
    ui->textEdit->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->textEdit, &QTextEdit::customContextMenuRequested, this, [&, contextMenu](const QPoint &pos) {
        contextMenu->exec(ui->textEdit->mapToGlobal(pos));
    });

    newFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
    saveFile(_path);
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
        _findDock = new QDockWidget(tr("Find"), this);
        _findDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

        _findDialog = new FindDialog(this, ui->textEdit);
        _findDock->setWidget(_findDialog);

        connect(_findDock, &QDockWidget::visibilityChanged, [&](bool visibility){
            if (!visibility)
                _findDialog->reset();
        });

        if (_replaceDock)
        {
            tabifyDockWidget(_replaceDock, _findDock);
            qApp->processEvents(); // Force QT to process events to tabify the widget and allow us to focus it directly
        }
        else
            addDockWidget(Qt::RightDockWidgetArea, _findDock);
    }
    else if (!_findDock->isVisible())
    {
        _findDock->setVisible(true);
    }

    _findDock->raise();
    _findDock->setFocus();

    // Update the search string with the current selection, if exist
    QTextCursor cursor = ui->textEdit->textCursor();
    QString selectedText = cursor.selectedText();
    if (!selectedText.isEmpty())
        _findDialog->setText(selectedText);

    _findDialog->setFocus();
}

void MainWindow::on_actionReplace_triggered()
{
    /*
    ReplaceDialog* dlg = new ReplaceDialog(this);
    if (!dlg->exec()) return;

    if (dlg->all())
    {
        QString text = ui->textEdit->toHtml();
        text = text.replace(dlg->text(), dlg->replaceText());
        ui->textEdit->setHtml(text);
    }
    else
    {
        bool value = ui->textEdit->find(dlg->text());
        QTextCursor cursor = ui->textEdit->textCursor();
        cursor.insertHtml(dlg->replaceText());
        if (!value) ui->statusbar->showMessage("Find '" + dlg->text() + "' not found, can't replace it");
    }*/

    if (_replaceDock == nullptr)
    {
        _replaceDock = new QDockWidget(tr("Replace"), this);
        _replaceDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

        _replaceDialog = new ReplaceDialog(this);
        _replaceDock->setWidget(_replaceDialog);

        connect(_replaceDock, &QDockWidget::visibilityChanged, [&](bool visibility){
            //if (!visibility)
            //    _replaceDialog->reset();
        });


        if (_findDock)
        {
            tabifyDockWidget(_findDock, _replaceDock);
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
    QTextCursor cursor = ui->textEdit->textCursor();
    QString selectedText = cursor.selectedText();
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
    QFont font = ui->textEdit->currentFont();
    font.bold() ? font.setBold(false) : font.setBold(true);
    ui->textEdit->setCurrentFont(font);
}

void MainWindow::on_actionItalic_triggered()
{
    QFont font = ui->textEdit->currentFont();
    font.italic() ? font.setItalic(false) : font.setItalic(true);
    ui->textEdit->setCurrentFont(font);
}

void MainWindow::on_actionUnderline_triggered()
{
    QFont font = ui->textEdit->currentFont();
    font.underline() ? font.setUnderline(false) : font.setUnderline(true);
    ui->textEdit->setCurrentFont(font);
}

void MainWindow::on_actionStrike_triggered()
{
    QFont font = ui->textEdit->currentFont();
    font.strikeOut() ? font.setStrikeOut(false) : font.setStrikeOut(true);
    ui->textEdit->setCurrentFont(font);
}

void MainWindow::on_actionColor_triggered()
{
    QColor currentColor = ui->textEdit->currentCharFormat().foreground().color();
    QColor color = QColorDialog::getColor(currentColor, this, "Select a color");

    ui->textEdit->setTextColor(color);
}

void MainWindow::on_actionFont_triggered()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, ui->textEdit->currentFont(), this, "Select a font");
    if (ok) ui->textEdit->setCurrentFont(font);
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog* dlg = new AboutDialog(this);
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
    _changed = true;
    updateCaption();
}

void MainWindow::on_actionPrint_triggered()
{
    QTextDocument *document = ui->textEdit->document();
    QPrinter printer;
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (dlg->exec() != QDialog::Accepted) return;
    document->print(&printer);
}

void MainWindow::updateCaption()
{
    this->setWindowTitle(_fileName + (_changed ? "*" : "") + " - " + QApplication::applicationDisplayName());
}

void MainWindow::newFile()
{
    ui->textEdit->clear();
    ui->statusbar->showMessage("New File");
    _fileName = "Untitled";
    _path = "";
    _changed = false;
    updateCaption();
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, "Open File");
    openFile(path);
}

void MainWindow::openFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this, "Error", file.errorString());
        return;
    }

    QTextStream stream(&file);
    ui->textEdit->setHtml(stream.readAll());
    file.close();

    _path = path;

    QFileInfo fileInfo(_path);
    _fileName = fileInfo.fileName();

    ui->statusbar->showMessage(_path);
    _changed = false;
     updateCaption();
    adjustForCurrentFile(_path);
}

void MainWindow::saveFile(QString path)
{
    if (path.isEmpty())
    {
        saveFileAs();
        return;
    }

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "Error", file.errorString());
        ui->statusbar->showMessage("Error, could not save the file");
        saveFileAs();
        return;
    }


    QTextStream stream(&file);
    stream << ui->textEdit->toHtml();
    file.close();

    _path = path;
    ui->statusbar->showMessage(_path);
    _changed = false;
    updateCaption();
}

void MainWindow::saveFileAs()
{
    QString path = QFileDialog::getSaveFileName(this, "Save File");
    if (path.isEmpty()) return;
    saveFile(path);
    adjustForCurrentFile(_path);
}

void MainWindow::checksave()
{
    if (!_changed) return;

    QMessageBox::StandardButton value = QMessageBox::question(this, "Save File?", "You have un-saved changes would you like to save them?");
    if (value != QMessageBox::StandardButton::Yes) return;

    if (_path.isEmpty())
    {
        saveFileAs();
    }
    else
    {
        saveFile(_path);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    checksave();
    event->accept();
}

void MainWindow::openRecent()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
        openFile(action->data().toString());
}

void MainWindow::adjustForCurrentFile(const QString& filePath)
{
    QSettings settings;
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();
    recentFilePaths.removeAll(filePath);
    recentFilePaths.prepend(filePath);

    while (recentFilePaths.size() > RECENT_FILES_MAX)
        recentFilePaths.removeLast();

    settings.setValue("recentFiles", recentFilePaths);

    updateRecentActionList();
}

void MainWindow::updateRecentActionList()
{
    QSettings settings;
    QStringList recentFilePaths = settings.value("recentFiles").toStringList();

    auto itEnd = 0u;
    if(recentFilePaths.size() <= RECENT_FILES_MAX)
        itEnd = recentFilePaths.size();
    else
        itEnd = RECENT_FILES_MAX;

    for (auto i = 0u; i < itEnd; ++i) {
        QString strippedName = QFileInfo(recentFilePaths.at(i)).fileName();
        _recentFileActionList.at(i)->setText(strippedName);
        _recentFileActionList.at(i)->setData(recentFilePaths.at(i));
        _recentFileActionList.at(i)->setVisible(true);
    }

    for (auto i = itEnd; i < RECENT_FILES_MAX; ++i)
        _recentFileActionList.at(i)->setVisible(false);
}
