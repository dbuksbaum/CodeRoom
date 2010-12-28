#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QString>
#include <QVector>
#include <QTextFormat>
#include <QFont>

class Highlighter : public QSyntaxHighlighter
{
    // Q_OBJECT macro
    Q_OBJECT
public:
    // Constructor
    Highlighter(QTextDocument *parent = 0);
    // Methods
    void loadSyntax(QString filename);
    void selectSyntax();
protected:
    void highlightBlock(const QString &text);
private:
    // Highlightingrule Struct
    struct HighlightingRule
    {
	QRegExp word,start,end;
	QTextCharFormat format;
    };
    // Vector of highlightingrules
    QVector<HighlightingRule> singleRules;
    QVector<HighlightingRule> multiRules;
};

#endif // HIGHLIGHTER_H
