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
    if ((event->key() == Qt::Key_Return && event->modifiers() == Qt::ControlModifier) || event->key() == Qt::Key_Escape) {
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
        updateCursor.insertText(editor->quickInsert[0].first);
        updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,editor->quickInsert[0].second);
        this->setTextCursor(updateCursor);
        break;
    case Qt::Key_F6:
        updateCursor.insertText(editor->quickInsert[1].first);
        updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,editor->quickInsert[1].second);
        this->setTextCursor(updateCursor);
        break;
    case Qt::Key_F7:
        updateCursor.insertText(editor->quickInsert[2].first);
        updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,editor->quickInsert[2].second);
        this->setTextCursor(updateCursor);
        break;
    case Qt::Key_F8:
        updateCursor.insertText(editor->quickInsert[3].first);
        updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,editor->quickInsert[3].second);
        this->setTextCursor(updateCursor);
        break;
    case Qt::Key_F9:
        updateCursor.insertText(editor->quickInsert[4].first);
        updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,editor->quickInsert[4].second);
        this->setTextCursor(updateCursor);
        break;
    case Qt::Key_F10:
        updateCursor.insertText(editor->quickInsert[5].first);
        updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,editor->quickInsert[5].second);
        this->setTextCursor(updateCursor);
        break;
    case Qt::Key_F11:
        updateCursor.insertText(editor->quickInsert[6].first);
        updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,editor->quickInsert[6].second);
        this->setTextCursor(updateCursor);
        break;
    case Qt::Key_F12:
        updateCursor.insertText(editor->quickInsert[7].first);
        updateCursor.movePosition(QTextCursor::Left,QTextCursor::MoveAnchor,editor->quickInsert[7].second);
        this->setTextCursor(updateCursor);
        break;
    }
}
