#include "highlighter.h"
#include <QColor>

Highlighter::Highlighter(Editor * editor, QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    // Save pointers to parents
    this->editor = editor;
    // Load Syntax highlighting
    this->loadSyntax("syntax/cpp.syn");
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
	    int endIndex;
	    if (previousBlockState() == blockState){
		endIndex = endExpression.indexIn(text, startIndex);
	    } else {
		endIndex = endExpression.indexIn(text, startIndex+1);
	    }
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

QString Highlighter::translate(QString str){
    str.replace("\\","\\\\");
    str.replace("?","\\?");
    str.replace("*","\\*");
    str.replace("^","\\^");
    str.replace("+","\\+");
    str.replace("|","\\|");
    str.replace("(","\\(");
    str.replace(")","\\)");
    str.replace("[","\\[");
    str.replace("]","\\]");
    str.replace("{","\\{");
    str.replace("}","\\}");
    str.replace("$","\\$");
    str.replace(".","\\.");
    return str;
}

void Highlighter::findFormatHelper(QString &data,HighlightingRule& rule,int i){
    int m,n;
    m = data.indexOf("color:",i) + 6;
    n = data.indexOf("\n",m);
    QString color = data.mid(m,n-m).trimmed();
    // Find format
    bool bold = false , italic = false , underline = false;
    m = data.indexOf("bold:",i) + 5;
    n = data.indexOf("\n",m);
    if (data.mid(m,n-m).trimmed() == "true") bold = true;
    m = data.indexOf("italic:",i) + 7;
    n = data.indexOf("\n",m);
    if (data.mid(m,n-m).trimmed() == "true") italic = true;
    m = data.indexOf("underline:",i) + 10;
    n = data.indexOf("\n",m);
    if (data.mid(m,n-m).trimmed() == "true") underline = true;
    // Set format
    rule.format.setForeground(QColor(color));
    if (bold) rule.format.setFontWeight(QFont::Bold);
    rule.format.setFontItalic(italic);
    rule.format.setFontUnderline(underline);
    // Initialize rest of rule
    rule.word = QRegExp("");
    rule.start = QRegExp("");
    rule.end = QRegExp("");
}

void Highlighter::loadSyntax(QString filename){
    // Load data
    QString data = editor->fileToStr(filename);
    // Find separator *******************************************
    int i = data.indexOf("Separator:");
    if (i == -1) return;
    QString sep = data.mid(i+10,1);
    // Find inlinewords *****************************************
    int j = 0;
    i = data.indexOf("inlinewords:");
    while (i != -1) {
	// Find format
	HighlightingRule rule;
	this->findFormatHelper(data,rule,i);
	// Find words
	int m = data.indexOf("\n",i) + 1;
	int end = data.indexOf("\n",m)+1;
	data.insert(end-1,sep);	    // Insert sep after last word
	int n = data.indexOf(sep,m);
	while (m < end){
	    // Append to singleRules
	    rule.word = QRegExp(this->translate(data.mid(m,n-m).trimmed()));
	    singleRules.append(rule);
	    // Set n and m
	    m = n + 1;
	    n = data.indexOf(sep,m);
	}
	// Update i and j
	j = i + 1;
	i = data.indexOf("inlinewords:",j);
    }
    // Find freewords *******************************************
    j = 0;
    i = data.indexOf("freewords:");
    while (i != -1) {
	// Find format
	HighlightingRule rule;
	this->findFormatHelper(data,rule,i);
	// Find words
	int m = data.indexOf("\n",i) + 1;
	int end = data.indexOf("\n",m)+1;
	data.insert(end-1,sep);	    // Insert sep after last word
	int n = data.indexOf(sep,m);
	while (m < end){
	    // Append to singleRules
	    QString tmp = "\\b";
	    tmp += this->translate(data.mid(m,n-m).trimmed());
	    tmp += "\\b";
	    rule.word = QRegExp(tmp);
	    singleRules.append(rule);
	    // Set n and m
	    m = n + 1;
	    n = data.indexOf(sep,m);
	}
	// Update i and j
	j = i + 1;
	i = data.indexOf("freewords:",j);
    }
    // Find singlelinespan **************************************
    j = 0;
    i = data.indexOf("singlelinespan:");
    while (i != -1) {
	// Find format
	HighlightingRule rule;
	this->findFormatHelper(data,rule,i);
	// Find words
	int m = data.indexOf("\n",i) + 1;
	int end = data.indexOf("\n",m)+1;
	data.insert(end-1,sep);	    // Insert sep after last word
	int n = data.indexOf(sep,m);
	while (m < end){
	    // Append to singleRules
	    QString tmp = this->translate(data.mid(m,n-m).trimmed());
	    tmp += "[^\n]*";
	    rule.word = QRegExp(tmp);
	    singleRules.append(rule);
	    // Set n and m
	    m = n + 1;
	    n = data.indexOf(sep,m);
	}
	// Update i and j
	j = i + 1;
	i = data.indexOf("singlelinespan:",j);
    }
    // Find multilinespan ***************************************
    j = 0;
    i = data.indexOf("multilinespan:");
    while (i != -1) {
	// Find format
	HighlightingRule rule;
	this->findFormatHelper(data,rule,i);
	// Find words
	int m = data.indexOf("\n",i) + 1;
	int end = data.indexOf("\n",m)+1;
	data.insert(end-1,sep);	    // Insert sep after last word
	int n = data.indexOf(sep,m);
	while (m < end){
	    // Append to singleRules
	    rule.start = QRegExp(this->translate(data.mid(m,n-m).trimmed()));
	    m = n + 3;
	    n = data.indexOf(sep,m);
	    rule.end = QRegExp(this->translate(data.mid(m,n-m).trimmed()));
	    multiRules.append(rule);
	    // Set n and m
	    m = n + 1;
	    n = data.indexOf(sep+sep+sep,m);
	}
	// Update i and j
	j = i + 1;
	i = data.indexOf("multilinespan:",j);
    }
}
