#ifndef TEXTFIND_H
#define TEXTFIND_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>

class TextFind : public QWidget
{
private:
    // Q_OBJECT macro
    Q_OBJECT
    // Objects
    QPushButton * btnFindNext;
    QPushButton * btnReplaceThis;
    QPushButton * btnReplaceAll;
    QLineEdit * txtFind;
    QLineEdit * txtReplace;
    QRadioButton * radUp;
    QRadioButton * radDown;
    QCheckBox * chkCase;
public:
    // Constructor
    TextFind(QWidget *parent = 0);
    // Destructor
    ~TextFind();
};

#endif // TEXTFIND_H
