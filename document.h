#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QTextEdit>
#include <QFile>
#include <QFileInfo>

class Document
{
public:
    Document();

    const bool isNew() const;

    const bool isSaved() const;
    void modified();

    const QString fullPath() const;
    bool setFullPath(const QString &fullPath);

    const QString fileName() const;
    const QString extension() const;

    void setEmpty();
    bool load(QTextEdit* textEdit);
    bool save(QTextEdit* textEdit);

    const QString getSupportedExtension() const;
    const QString getSupportedExtensionFilter() const;

private:
    bool checkExtension(const QString& extension);

private:
    bool _isSaved;
    QString _fileName;
    QString _fullPath;
    QString _extension;
};

#endif // DOCUMENT_H
