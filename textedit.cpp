#include "textedit.h"
#include <QProcess>

using namespace std;

TextEdit::TextEdit(Editor * parent) : QTextEdit(parent){
    // Store pointer to editor
    editor = parent;
    // Only accept plain text
    this->setAcceptRichText(false);
    // Hide horizonal scrollbar
    this->horizontalScrollBar()->hide();
}

void TextEdit::keyPressEvent(QKeyEvent * event){
    // Keys to skip
    skipKey = false;
    skipKeys << Qt::Key_F1 << Qt::Key_F10 << Qt::Key_F5 << Qt::Key_F6 << Qt::Key_F7 << Qt::Key_F8 << Qt::Key_F9 << Qt::Key_F10 << Qt::Key_F11 << Qt::Key_F12;
    for (int i = 0; i != skipKeys.size(); i++){
        if (event->key() == skipKeys[i]){
            skipKey = true;
            break;
        }
    }
    // Keep functionality of QTextEdit (but skip skipKeys)
    if (!skipKey) {
        QTextEdit::keyPressEvent(event);
    }
    // Text cursor
    QTextCursor updateCursor = this->textCursor();
    // End + Newline
    if (event->key() == Qt::Key_Return && event->modifiers() == Qt::ControlModifier) {
        updateCursor.movePosition(QTextCursor::EndOfBlock,QTextCursor::MoveAnchor);
        updateCursor.insertText("\n");
        this->setTextCursor(updateCursor);
    }
    // Auto-indentation
    if (event->key() == Qt::Key_Return && (event->modifiers() == Qt::NoModifier || event->modifiers() == Qt::ControlModifier || event->modifiers() == Qt::ShiftModifier)) {
	QString data = editor->dataToStr();
        int cursorPosition = updateCursor.position();
	int i;
	for (i=cursorPosition-2; i>=0; i--){
	    if (data.mid(i,1) == "\n") {
		break;
	    }
	}
	while (data.mid(i+1,1) == "\t"){
            updateCursor.insertText("\t");
	    i++;
	}
    }
    // Open readme
    if (event->key() == Qt::Key_F1) {
        if (editor->checkAndSave()){
            editor->openFile("config.cfg");
        }
    }
    // Save file
    if (event->key() == Qt::Key_S && event->modifiers() == Qt::ControlModifier) {
        editor->saveFile();
    }
    // Save file as
    if (event->key() == Qt::Key_W && event->modifiers() == Qt::ControlModifier) {
	editor->saveFileAs();
    }
    // Open file
    if (event->key() == Qt::Key_O && event->modifiers() == Qt::ControlModifier) {
	if (editor->checkAndSave()){
	    editor->openFile();
	}
    }
    // New file
    if (event->key() == Qt::Key_N && event->modifiers() == Qt::ControlModifier) {
	if (editor->checkAndSave()){
	    editor->newFile();
	}
    }
    // Set highlighting
    if (event->key() == Qt::Key_H && event->modifiers() == Qt::ControlModifier) {
	editor->chooseSyntax();
    }
    // Quit
    if ((event->key() == Qt::Key_Q && event->modifiers() == Qt::ControlModifier) || event->key() == Qt::Key_Escape) {
	if (editor->checkAndSave()){
	    qApp->quit();
	}
    }
    // Shortcut keys / quickinsert
    int keyF=-1;
    switch (event->key()){
    case Qt::Key_F5:
        keyF = 0;
        break;
    case Qt::Key_F6:
        keyF = 1;
        break;
    case Qt::Key_F7:
        keyF = 2;
        break;
    case Qt::Key_F8:
        keyF = 3;
        break;
    case Qt::Key_F9:
        keyF = 4;
        break;
    case Qt::Key_F10:
        keyF = 5;
        break;
    case Qt::Key_F11:
        keyF = 6;
        break;
    case Qt::Key_F12:
        keyF = 7;
        break;
    }
    // Actual insert + set on the fly
    if (keyF != -1 && event->modifiers() == Qt::ControlModifier){
        QString tmp = updateCursor.selectedText().trimmed();
        tmp.replace("\\n","\n").replace("\\\n","\\n");
        tmp.replace("\\t","\t").replace("\\\t","\\t");
        editor->quickInsert[keyF].first = tmp.mid(1);
        editor->quickInsert[keyF].second = tmp.mid(0,1);
        updateCursor.removeSelectedText();
    } else if (keyF != -1) {
        this->quickInsert(updateCursor,editor->quickInsert[keyF].first,editor->quickInsert[keyF].second);
    }
}

void TextEdit::quickInsert(QTextCursor & updateCursor, QString tmp, QString sep){
    // Needed variables
    QString sel,lin,wrd;
    QString data = this->toPlainText();
    int i,j,k,start;
    // Get selection
    sel = updateCursor.selectedText();
    updateCursor.removeSelectedText();
    // Start position
    start = updateCursor.position();
    // Get line
    i = updateCursor.position()-1;
    j = i;
    while (data.mid(i,1) != "\n" && i != -1) i--;
    i++;
    while (data.mid(j,1) != "\n" && j <= data.size()) j++;
    j--;
    lin = (j-i+1 > 0) ? data.mid(i,j-i+1) : "";
    // Get word
    i = updateCursor.position()-1;
    j = i;
    while ((data.mid(i,1) != " " && data.mid(i,1) != "\n" && data.mid(i,1) != "\t") && i != -1) i--;
    i++;
    while ((data.mid(j,1) != " " && data.mid(j,1) != "\n" && data.mid(j,1) != "\t") && j <= data.size()) j++;
    j--;
    wrd = (j-i+1 > 0) ? data.mid(i,j-i+1) : "";
    // Replace parameters and fix space
    tmp.replace(sep,"\x99");
    tmp.replace("\x99\x99\x99\x99","\x93").replace("\x99\x99\x99","\x92").replace("\x99\x99","\x91").replace("\x99","\x90");
    tmp.replace(" ","").replace("\\\\s","\x98").replace("\\s"," ").replace("\x98","\\s");
    // Delete line if lin exists
    if (tmp.indexOf("\x92") != -1 && lin != ""){
	j++;
	updateCursor.setPosition(j);
        while (!updateCursor.atBlockStart()) updateCursor.deletePreviousChar();
    } else if (tmp.indexOf("\x93") != -1 && wrd != "") {
        j++;
        updateCursor.setPosition(j);
        while (updateCursor.position() != 0 && data.mid(updateCursor.position()-1,1) != " "
               && data.mid(updateCursor.position()-1,1) != "\n" && data.mid(updateCursor.position()-1,1) != "\t") updateCursor.deletePreviousChar();
    }
    //if (lin == "\n") lin = "";
    if (tmp.indexOf("\x92") || tmp.indexOf("\x93") != -1) start = updateCursor.position();
    tmp.replace("\x93",wrd).replace("\x92",lin).replace("\x91",sel);
    k = tmp.size()-tmp.indexOf("\x90")-1;
    tmp.replace("\x90","");
    // Insert and move cursor
    updateCursor.setPosition(start);
    updateCursor.insertText(tmp);
    updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,k);
    this->setTextCursor(updateCursor);
}
