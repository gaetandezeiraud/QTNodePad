#include "searchhighlight.h"

#include <QTextCharFormat>

SearchHighLight::SearchHighLight(QTextDocument* parent) : QSyntaxHighlighter(parent)
{
    // Set the backlight color
    _format.setBackground(Qt::yellow);
}

void SearchHighLight::highlightBlock(const QString& text)
{
    // Using a regular expression, we find all matches.
    auto matchIterator = _pattern.globalMatch(text);
    while (matchIterator.hasNext())
    {
        // Highlight all matches
        const auto match = matchIterator.next();
        setFormat(match.capturedStart(), match.capturedLength(), _format);
    }
}

void SearchHighLight::setCaseSensitive(bool newCaseSensitive)
{
    _caseSensitive = newCaseSensitive;
}

void SearchHighLight::setWholeWord(bool newWholeWord)
{
    _wholeWord = newWholeWord;
}

void SearchHighLight::searchText(const QString& text)
{
    // Set the text as a regular expression.
    QString regexExpr = _wholeWord ? "\\b" : "";

    _pattern = QRegularExpression(regexExpr + text + regexExpr);
    if (!_caseSensitive)
        _pattern.setPatternOptions(QRegularExpression::CaseInsensitiveOption);

    rehighlight(); // Restart the backlight
}
