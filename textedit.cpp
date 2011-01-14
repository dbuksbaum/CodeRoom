#include "textedit.h"

using namespace std;

TextEdit::TextEdit(Editor * parent) : QTextEdit(parent){
    // Store pointer to editor
    editor = parent;
    // Only accept plain text
    this->setAcceptRichText(false);
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
    switch (event->key()){
    case Qt::Key_F5:
        this->quickInsert(updateCursor,editor->quickInsert[0].first,editor->quickInsert[0].second);
        break;
    case Qt::Key_F6:
        this->quickInsert(updateCursor,editor->quickInsert[1].first,editor->quickInsert[1].second);
        break;
    case Qt::Key_F7:
        this->quickInsert(updateCursor,editor->quickInsert[2].first,editor->quickInsert[2].second);
        break;
    case Qt::Key_F8:
        this->quickInsert(updateCursor,editor->quickInsert[3].first,editor->quickInsert[3].second);
        break;
    case Qt::Key_F9:
        this->quickInsert(updateCursor,editor->quickInsert[4].first,editor->quickInsert[4].second);
        break;
    case Qt::Key_F10:
        this->quickInsert(updateCursor,editor->quickInsert[5].first,editor->quickInsert[5].second);
        break;
    case Qt::Key_F11:
        this->quickInsert(updateCursor,editor->quickInsert[6].first,editor->quickInsert[6].second);
        break;
    case Qt::Key_F12:
        this->quickInsert(updateCursor,editor->quickInsert[7].first,editor->quickInsert[7].second);
        break;
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
    tmp = tmp.replace(sep,"\x99");
    while (!updateCursor.atBlockStart() && tmp.indexOf("\x99\x99\x99") != -1) updateCursor.deletePreviousChar();
    tmp = tmp.replace("\x99\x99\x99",lin).replace("\x99\x99",sel);
    tmp = tmp.replace(" ","").replace("\\\\s","\x98").replace("\\s"," ").replace("\x98","\\s");
    k = tmp.size()-tmp.indexOf("\x99")-1;
    tmp.replace("\x99","");
    // Insert and move cursor
    updateCursor.setPosition(start);
    updateCursor.insertText(tmp);
    updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,k);
    this->setTextCursor(updateCursor);
}
