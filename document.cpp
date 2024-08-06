#include "document.h"

#include <QMessageBox>

Document::Document()
    : _fullPath{}
    , _fileName{"Untitled"}
    , _extension{}
    , _isSaved{true}
{

}

const bool Document::isNew() const
{
    return _fullPath.isEmpty();
}

const bool Document::isSaved() const
{
    return _isSaved;
}

void Document::modified()
{
    _isSaved = false;
}

const QString Document::fullPath() const
{
    return _fullPath;
}

void Document::setFullPath(const QString &fullPath)
{
    _fullPath = fullPath;

    QFileInfo fileInfo(_fullPath);
    _fileName = fileInfo.fileName();
    _extension = fileInfo.suffix();
}

const QString Document::fileName() const
{
    return _fileName;
}

const QString Document::extension() const
{
    return _extension;
}

bool Document::load(const QString& fullPath, QTextEdit* textEdit)
{
    QFile file(fullPath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QTextStream stream(&file);
    textEdit->setHtml(stream.readAll());
    file.close();

    setFullPath(fullPath);

    return true;
}

bool Document::save(QTextEdit* textEdit)
{
    if (_fullPath.isEmpty())
        return false;

    QFile file(_fullPath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    QTextStream stream(&file);
    stream << textEdit->toHtml();
    file.close();

    _isSaved = true;
    return true;
}
