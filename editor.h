#ifndef EDITOR_H
#define EDITOR_H

#include "textedit.h"
#include "highlighter.h"
#include <QApplication>
#include <QWidget>
#include <QTextEdit>
#include <QLayout>
#include <QSpacerItem>
#include <QKeyEvent>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>

class TextEdit;
class Highlighter;

class Editor : public QWidget
{
private:
    // Q_OBJECT macro
    Q_OBJECT
    // Objects
    TextEdit * textField;
    Highlighter * highlighter;
    QHBoxLayout * textLayout;
    // Variables
    QString openFilename;
    bool fileIsOpen;
    bool fileIsSaved;
public:
    // Constructors
    Editor(QWidget *parent = 0);
    // Destructors
    ~Editor();
    // Methods
    QString fileToStr(QString filename);
    QString dataToStr();
    void strToFile(QString str, QString filename);
    void strToData(QString str);
    void loadStyle(QString filename);
    void openFile();
    void saveFile();
    void saveFileAs();
    void newFile();
    bool checkAndSave();
};

#endif // EDITOR_H
