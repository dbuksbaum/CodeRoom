#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include "editor.h"
#include <QTextEdit>
#include <QKeyEvent>
#include <QString>
#include <QVector>
#include <QScrollBar>

class Editor;

class TextEdit : public QTextEdit
{
private:
    // Q_OBJECT macro
    Q_OBJECT
    // Pointer to editor
    Editor * editor;
    // Other variables and objects
    QVector<int> skipKeys;
    bool skipKey;
    // Private methods
    void quickInsert(QTextCursor & updateCursor, QString tmp, QString sep);
protected:
    // On keypress
    void keyPressEvent(QKeyEvent * event);
public:
    // Constructor
    TextEdit(Editor * parent = 0);
};

#endif // TEXTEDIT_H
