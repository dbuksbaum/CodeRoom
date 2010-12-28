#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    // Helper objects
    HighlightingRule rule;
    // Set pattern + format
    // Single word
    rule.format.setForeground(Qt::darkBlue);
    rule.format.setFontWeight(QFont::Bold);
    rule.format.setFontItalic(false);
    rule.word = QRegExp("\\bmain\\b");
    rule.start = QRegExp("");
    rule.end = QRegExp("");
    singleRules.append(rule);
    // Single line
    rule.format.setForeground(Qt::yellow);
    rule.format.setFontWeight(QFont::Normal);
    rule.format.setFontItalic(false);
    rule.word = QRegExp("//[^\n]*");
    rule.start = QRegExp("");
    rule.end = QRegExp("");
    singleRules.append(rule);
    // Multiline
    rule.format.setForeground(Qt::white);
    rule.format.setFontWeight(QFont::Normal);
    rule.format.setFontItalic(true);
    rule.word = QRegExp("");
    rule.start = QRegExp("\"");
    rule.end = QRegExp("\"");
    multiRules.append(rule);
    // Multiline
    rule.format.setForeground(Qt::red);
    rule.format.setFontWeight(QFont::Normal);
    rule.format.setFontItalic(true);
    rule.word = QRegExp("");
    rule.start = QRegExp("/\\*");
    rule.end = QRegExp("\\*/");
    multiRules.append(rule);
}

void Highlighter::highlightBlock(const QString &text)
{
    // Single word highlights
    foreach (const HighlightingRule &rule, singleRules) {
	QRegExp expression(rule.word);
	int index = expression.indexIn(text);
	while (index >= 0) {
	    int length = expression.matchedLength();
	    setFormat(index, length, rule.format);
	    index = expression.indexIn(text, index + length);
	}
    }
    // Multiple line highlight
    int blockState = 1;
    setCurrentBlockState(0);
    foreach (const HighlightingRule &rule, multiRules) {
	QRegExp startExpression(rule.start);
	QRegExp endExpression(rule.end);
	int startIndex = 0;
	if (previousBlockState() != blockState) startIndex = startExpression.indexIn(text);
	while (startIndex >= 0) {
	    int endIndex = endExpression.indexIn(text, startIndex+1);
	    int multiLength;
	    if (endIndex == -1) {
		setCurrentBlockState(blockState);
		multiLength = text.length() - startIndex;
	    } else {
		multiLength = endIndex - startIndex + endExpression.matchedLength();
	    }
	    setFormat(startIndex, multiLength, rule.format);
	    startIndex = startExpression.indexIn(text, startIndex + multiLength);
	}
	blockState++;
    }
}

void Highlighter::loadSyntax(QString filename){
    //
}
