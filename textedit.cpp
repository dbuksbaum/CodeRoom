#include "textedit.h"

using namespace std;

TextEdit::TextEdit(Editor * parent) : QTextEdit(parent){
    // Store pointer to editor
    editor = parent;
}

void TextEdit::keyPressEvent(QKeyEvent * event){
    // Keep functionality of QTextEdit
    QTextEdit::keyPressEvent(event);
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
	editor->openFile();
    }
    // New file
    if (event->key() == Qt::Key_N && event->modifiers() == Qt::ControlModifier) {
	editor->newFile();
    }
    // Quit
    if (event->key() == Qt::Key_Q && event->modifiers() == Qt::ControlModifier) {
	qApp->quit();
    }
}
