#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include "editor.h"
#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextEdit>
#include <QString>
#include <QVector>
#include <QTextFormat>
#include <QFont>
#include <QColor>

class Editor;

class Highlighter : public QSyntaxHighlighter
{
private:
    // Q_OBJECT macro
    Q_OBJECT
    // Highlightingrule Struct
    struct HighlightingRule
    {
	QRegExp word,start,end;
	QTextCharFormat format;
    };
    // Vector of highlightingrules
    QVector<HighlightingRule> multiRules;
    QVector<HighlightingRule> singleRules;
    // Pointer to editor
    Editor * editor;
    // Helper method
    void findFormatHelper(QString& data,HighlightingRule& rule,int i);
protected:
    void highlightBlock(const QString &text);
public:
    // Constructor
    Highlighter(Editor * editor, QTextDocument *parent = 0);
    // Methods
    void loadSyntax(QString filename);
    void selectSyntax();
    QString translate(QString str);
};

#endif // HIGHLIGHTER_H
