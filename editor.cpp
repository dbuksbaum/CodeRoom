#include "editor.h"
#include <QMessageBox>

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    // Apply style
    this->loadStyle("style.sty");
    // Initialize text field
    textField = new TextEdit(this);
    textField->setTabStopWidth(20);  // Tab width
    // Add highlighter
    highlighter = new Highlighter(this, textField->document());
    // Set up layout
    textLayout = new QHBoxLayout();
    int textFieldStretch = 35;
    int sideStretch = (100-textFieldStretch)/2;
    textLayout->addStretch(sideStretch);
    textLayout->addWidget(textField,textFieldStretch);
    textLayout->addStretch(sideStretch);
    this->setLayout(textLayout);
    // No file is open
    fileIsOpen = false;
    openFilename = "";
}

Editor::~Editor(){
    delete textField;
    delete textLayout;
}

void Editor::loadStyle(QString filename){
    // Open file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    // Create stream and load data
    QTextStream in(&file);
    QString style = "";
    while (!in.atEnd()) {
	QString line = in.readLine();
	style.append(line);
    }
    file.close();
    // Apply style
    qApp->setStyleSheet(style);
}

void Editor::openFile(){
    // Load data
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"), "", tr("All Files (*)"));
    // Check if filename is empty
    if (filename == "") return;
    // Set data
    this->strToData(this->fileToStr(filename));
    // File is open
    fileIsOpen = true;
    openFilename = filename;
}

void Editor::saveFile(){
    if (fileIsOpen){
	// Save to open file
	this->strToFile(this->dataToStr(), openFilename);
    } else {
	// Save as
	this->saveFileAs();
    }
}

void Editor::saveFileAs(){
    // Save as
    QString filename = QFileDialog::getSaveFileName(this,tr("Save As"), "", tr("All Files (*)"));
    // Check if filename is empty
    if (filename == "") return;
    // Save file
    this->strToFile(this->dataToStr(), filename);
    fileIsOpen = true;
    openFilename = filename;
}

void Editor::newFile(){
    // Clear text field
    this->strToData("");
    // Erase memory
    fileIsOpen = false;
    openFilename = "";
}

QString Editor::dataToStr(){
    // Convert current data to plain text string
    return textField->toPlainText();
}

void Editor::strToFile(QString str, QString filename){
    // Open file
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;
    // Create stream and write data
    QTextStream out(&file);
    out << str;
    file.close();
}

void Editor::strToData(QString str){
    textField->setText(str);
}

QString Editor::fileToStr(QString filename){
    // Open file
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return "Error opening file.";
    // Create stream and load data
    QTextStream in(&file);
    QString str = "";
    while (!in.atEnd()) {
	QString line = in.readLine();
	str.append(line);
	str.append("\n");
    }
    file.close();
    // Return string
    return str;
}

bool Editor::checkAndSave(){
    if ( (fileIsOpen && textField->toPlainText() != this->fileToStr(openFilename)) || (!fileIsOpen && !textField->toPlainText().isEmpty()) ){
	QMessageBox saveBox;
	saveBox.setText("The document has been modified.");
	saveBox.setInformativeText("Do you want to save your changes?");
	saveBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
	saveBox.setDefaultButton(QMessageBox::Save);
	int save = saveBox.exec();
	switch (save) {
	case QMessageBox::Save:
	    this->saveFile();
	    return true;
	case QMessageBox::Discard:
	    return true;
	case QMessageBox::Cancel:
	    return false;
	default:
	    return false;
	}
    }
    // If file is unchanged, allow action
    return true;
}
