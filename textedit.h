#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "editor.h"
#include <QTextEdit>
#include <QKeyEvent>
#include <QString>

class Editor;

class TextEdit : public QTextEdit
{
private:
    // Q_OBJECT macro
    Q_OBJECT
    // Pointer to editor
    Editor * editor;
protected:
    // On keypress
    void keyPressEvent(QKeyEvent * event);
public:
    // Constructor
    TextEdit(Editor * parent = 0);
};

#endif // TEXTEDIT_H
