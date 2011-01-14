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
#include <QMessageBox>
#include <QPair>

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
public:
    // Constructors
    Editor(QWidget *parent = 0);
    // Destructors
    ~Editor();
    // Methods
    QString fileToStr(QString filename);
    QString dataToStr();
    QString getConfHelper(QString str, QString conf);
    void strToFile(QString str, QString filename);
    void strToData(QString str);
    void setStyle();
    void setSyntax(QString filename);
    void chooseSyntax();
    void openFile();
    void openFile(QString filename);
    void saveFile();
    void saveFileAs();
    void newFile();
    bool checkAndSave();
    // Public shortcut vector
    QVector< QPair<QString,QString> > quickInsert;
};

#endif // EDITOR_H
