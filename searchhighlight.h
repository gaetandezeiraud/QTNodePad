#ifndef SEARCHHIGHLIGHT_H
#define SEARCHHIGHLIGHT_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class SearchHighLight : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    explicit SearchHighLight(QTextDocument* parent = nullptr);

    void searchText(const QString& text);

    void setWholeWord(bool newWholeWord);

    void setCaseSensitive(bool newCaseSensitive);

protected:
    virtual void highlightBlock(const QString &text) override;

private:
    QRegularExpression _pattern; // Regular expression to search for, in our case, this word or text
    QTextCharFormat _format;     // Text formatting, highlighting

    bool _wholeWord = false;
    bool _caseSensitive = false;
};

#endif // SEARCHHIGHLIGHT_H
