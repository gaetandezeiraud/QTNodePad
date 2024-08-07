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

bool Document::setFullPath(const QString &fullPath)
{
    QFileInfo fileInfo(fullPath);
    QString extension = fileInfo.suffix();

    if (!checkExtension(extension))
        return false;

    _fullPath = fullPath;
    _fileName = fileInfo.fileName();
    _extension = fileInfo.suffix();

    return true;
}

const QString Document::fileName() const
{
    return _fileName;
}

const QString Document::extension() const
{
    return _extension;
}

bool Document::load(QTextEdit* textEdit)
{
    QFile file(_fullPath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QTextStream stream(&file);
    textEdit->setHtml(stream.readAll());
    file.close();

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

const QString Document::getSupportedExtension() const
{
    return "qnp";
}

const QString Document::getSupportedExtensionFilter() const
{
    return "QTNodePad (*.qnp);;";
}

bool Document::checkExtension(const QString& extension)
{
    return extension == getSupportedExtension();
}
