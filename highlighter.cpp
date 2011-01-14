#include "highlighter.h"

Highlighter::Highlighter(Editor * editor, QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    // Save pointers to parents
    this->editor = editor;
}

void Highlighter::highlightBlock(const QString &text)
{
    // Single word highlights
    foreach (const HighlightingRule &rule, singleRules) {
	QRegExp expression(rule.word);
	QRegExp notBehind(rule.start);
	int index = expression.indexIn(text);
	while (index >= 0) {
	    // Workaround for negative lookbehind **
	    notBehind.indexIn(text);
	    int behindIndex = index - notBehind.matchedLength();
	    if (notBehind != QRegExp("") && behindIndex >= 0 && notBehind.indexIn(text.mid(behindIndex,notBehind.matchedLength())) != -1){
		index = expression.indexIn(text,index + 1);
		continue;
	    }
	    // *************************************
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
	QRegExp notBehind(rule.word);
	int startIndex = 0;
	if (previousBlockState() != blockState) startIndex = startExpression.indexIn(text);
	while (startIndex >= 0) {
	    int endIndex;
	    // Workaround for negative lookbehind **
	    notBehind.indexIn(text);
	    int behindIndex = startIndex - notBehind.matchedLength();
	    if (notBehind != QRegExp("") && behindIndex >= 0 && notBehind.indexIn(text.mid(behindIndex,notBehind.matchedLength())) != -1){
		startIndex = startExpression.indexIn(text,startIndex+1);
		continue;
	    }
	    // *************************************
	    if (previousBlockState() == blockState){
		if (startIndex == 0) endIndex = endExpression.indexIn(text, startIndex);
		else endIndex = endExpression.indexIn(text, startIndex+startExpression.matchedLength());
	    } else {
		endIndex = endExpression.indexIn(text, startIndex+startExpression.matchedLength());
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
    str.replace("\\\\s"," ");
    str.replace("\\\\n","\n");
    str.replace("\\\\t","\t");
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
    // Clear rules
    singleRules.clear();
    multiRules.clear();
    // Load data
    QString data;
    try {
	data = editor->fileToStr(filename);
        data.remove(QRegExp("[\n][^:]*[\n][/][/][^\n]*"));
    } catch (QString) {
	QString tmp = editor->dataToStr();
	if (tmp != "") tmp += "\n";
	tmp += "Syntax highlighting file, does not exists";
	editor->strToData(tmp);
	data = "";
    }
    // Find separator *******************************************
    int i = data.indexOf("separator:");
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
	    m = n + 3;
	    n = data.indexOf(sep,m);
	    tmp += "[^";
	    tmp += this->translate(data.mid(m,n-m).trimmed());
	    tmp +="]*";
	    tmp += this->translate(data.mid(m,n-m).trimmed());
	    rule.word = QRegExp(tmp);
	    singleRules.append(rule);
	    // Set n and m
	    m = n + 1;
	    n = data.indexOf(sep+sep+sep,m);
	}
	// Update i and j
	j = i + 1;
	i = data.indexOf("singlelinespan:",j);
    }
    // Find restofline ******************************************
    j = 0;
    i = data.indexOf("restofline:");
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
	i = data.indexOf("restofline:",j);
    }
    // Find singleregexp ******************************************
    j = 0;
    i = data.indexOf("singleregexp:");
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
	    rule.word = QRegExp(data.mid(m,n-m).trimmed());
	    singleRules.append(rule);
	    // Set n and m
	    m = n + 1;
	    n = data.indexOf(sep,m);
	}
	// Update i and j
	j = i + 1;
	i = data.indexOf("singleregexp:",j);
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
    // Find multiregexp ***************************************
    j = 0;
    i = data.indexOf("multiregexp:");
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
	    rule.start = QRegExp(data.mid(m,n-m).trimmed());
	    m = n + 3;
	    n = data.indexOf(sep,m);
	    rule.end = QRegExp(data.mid(m,n-m).trimmed());
	    multiRules.append(rule);
	    // Set n and m
	    m = n + 1;
	    n = data.indexOf(sep+sep+sep,m);
	}
	// Update i and j
	j = i + 1;
	i = data.indexOf("multiregexp:",j);
    }
    // Find singlebehindregexp **************************************
    j = 0;
    i = data.indexOf("singlebehindregexp:");
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
	    rule.start = QRegExp(data.mid(m,n-m).trimmed());
	    m = n + 3;
	    n = data.indexOf(sep,m);
	    rule.word = QRegExp(data.mid(m,n-m).trimmed());
	    singleRules.append(rule);
	    // Set n and m
	    m = n + 1;
	    n = data.indexOf(sep+sep+sep,m);
	}
	// Update i and j
	j = i + 1;
	i = data.indexOf("singlebehindregexp:",j);
    }
    // Find multibehindregexp ***************************************
    j = 0;
    i = data.indexOf("multibehindregexp:");
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
	    rule.word = QRegExp(data.mid(m,n-m).trimmed());
	    m = n + 3;
	    n = data.indexOf(sep,m);
	    rule.start = QRegExp(data.mid(m,n-m).trimmed());
	    m = n + 3;
	    n = data.indexOf(sep,m);
	    rule.end = QRegExp(data.mid(m,n-m).trimmed());
	    multiRules.append(rule);
	    // Set n and m
	    m = n + 1;
	    n = data.indexOf(sep+sep+sep,m);
	}
	// Update i and j
	j = i + 1;
	i = data.indexOf("multibehindregexp:",j);
    }
    // Find quickinsert *********************************************
    int m = 0;
    i = data.indexOf("quickinsert:");
    if (i != -1) {
        j = data.indexOf("\n",i);
        i = data.indexOf("\n",j+1);
    }
    while (i != -1) {
        if (data.mid(j+1,i-j-1).trimmed() == "") break;
        QString tmp = data.mid(j+1,i-j-1).trimmed();
        tmp.replace("\\n","\n").replace("\\\n","\\n");
        tmp.replace("\\t","\t").replace("\\\t","\\t");
        editor->quickInsert[m].first = tmp;
        editor->quickInsert[m].second = sep;
        j = i;
        i = data.indexOf("\n",j+1);
        m++;
    }
}
