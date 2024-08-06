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
    void setFullPath(const QString &fullPath);

    const QString fileName() const;
    const QString extension() const;

    void setEmpty();
    bool load(const QString& fullPath, QTextEdit* textEdit);
    bool save(QTextEdit* textEdit);

private:
    bool _isSaved;
    QString _fileName;
    QString _fullPath;
    QString _extension;
};

#endif // DOCUMENT_H
