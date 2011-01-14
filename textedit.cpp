#include "textedit.h"
#include <QDebug>
#include <QScrollBar>

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
    skipKeys << Qt::Key_F10 << Qt::Key_F5 << Qt::Key_F6 << Qt::Key_F7 << Qt::Key_F8 << Qt::Key_F9 << Qt::Key_F10 << Qt::Key_F11 << Qt::Key_F12;
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
    QString sel,lin;
    int i,j,k,start;
    // Get selection
    sel = updateCursor.selectedText();
    updateCursor.removeSelectedText();
    // Start position
    start = updateCursor.position();
    // Get line
    while (!updateCursor.atBlockStart()) updateCursor.movePosition(QTextCursor::Left);
    i = updateCursor.position();
    while (!updateCursor.atBlockEnd()) updateCursor.movePosition(QTextCursor::Right);
    j = updateCursor.position();
    lin = this->toPlainText().mid(i,j-i+1);
    // Replace parameters and fix space
    tmp.replace(sep,"\x99");
    while (!updateCursor.atBlockStart() && tmp.indexOf("\x99\x99\x99") != -1) updateCursor.deletePreviousChar();
    tmp.replace("\x99\x99\x99",lin).replace("\x99\x99",sel);
    tmp.replace(" ","").replace("\\\\s","\x98").replace("\\s"," ").replace("\x98","\\s");
    k = tmp.size()-tmp.indexOf("\x99")-1;
    tmp.replace("\x99","");
    // Insert and move cursor
    updateCursor.setPosition(start);
    updateCursor.insertText(tmp);
    updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,k);
    this->setTextCursor(updateCursor);
}
