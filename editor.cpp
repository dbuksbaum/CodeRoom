#include "editor.h"

Editor::Editor(QWidget *parent) : QWidget(parent)
{
    // Initialize text field
    textField = new TextEdit(this);
    // Add highlighter
    highlighter = new Highlighter(this, textField->document());
    // Set up layout
    textLayout = new QHBoxLayout();
    this->setLayout(textLayout);
    textLayout->addStretch();		    // Add spacing
    textLayout->addWidget(textField);	    // Add textfield to layout
    textLayout->addStretch();		    // add spacing
    // Apply style and configurations
    this->setStyle();
    // No file is open
    fileIsOpen = false;
    openFilename = "";
    // Initialize shortcut vector
    quickInsert = QVector< QPair<QString,int> >(8,QPair<QString,int>("",0));
}

Editor::~Editor(){
    delete textField;
    delete textLayout;
}

QString Editor::getConfHelper(QString str, QString conf){
    int n, m;
    m = conf.indexOf(str);
    n = conf.indexOf(":",m)+1;
    m = conf.indexOf("\n",n);
    return conf.mid(n,m-n).trimmed();
}

void Editor::setStyle(){
    // Open config and stylesheet files
    QString style, conf;
    try {
	style = this->fileToStr("style.sty");
	conf = this->fileToStr("config.cfg");
    } catch (QString) {
	QString tmp = this->dataToStr();
	if (tmp != "") tmp += "\n";
	tmp += "Error opening config.cfg or style.sty";
	this->strToData(tmp);
	style = "";
	conf = "";
    }
    // Get width
    int widthPercentage = this->getConfHelper("Width-percentage:",conf).toInt();
    // Get tab-width
    int tabWidth = this->getConfHelper("Tab-width:",conf).toInt();
    // Set configurations
    textField->setTabStopWidth(tabWidth);		    // Tab width
    textLayout->setStretch(0,(100-widthPercentage)/2);	    // Set spacing stretch
    textLayout->setStretch(1,widthPercentage);		    // Set textField stretch
    textLayout->setStretch(2,(100-widthPercentage)/2);	    // Set spacing stretch
    // Get and set colors
    style.replace("BGCOLOR",this->getConfHelper("Background-color:",conf));
    style.replace("SCCOLOR",this->getConfHelper("Scroll-color:",conf));
    style.replace("LACOLOR",this->getConfHelper("Light-app-color:",conf));
    style.replace("DACOLOR",this->getConfHelper("Dark-app-color:",conf));
    style.replace("FCOLOR",this->getConfHelper("Font-color:",conf));
    style.replace("SCOLOR",this->getConfHelper("Selected-font-color:",conf));
    style.replace("SBCOLOR",this->getConfHelper("Selected-background-color:",conf));
    // Get font styles
    style.replace("FFAMILY",this->getConfHelper("Font:",conf));
    style.replace("FSIZE",this->getConfHelper("Font-size:",conf));
    style.replace("FBOLD",( (this->getConfHelper("Font-bold:",conf) == "true") ? "bold" : "normal" ));
    style.replace("FITALIC",( (this->getConfHelper("Font-italic:",conf) == "true") ? "italic" : "normal" ));
    style.replace("FUNDERLINE",( (this->getConfHelper("Font-underline:",conf) == "true") ? "underline" : "none" ));
    // Apply style
    qApp->setStyleSheet(style);
}

void Editor::setSyntax(QString filename){
    // Find extension
    QString filetype;
    for (int i=filename.length()-1; i>=0; i--){
	if (filename.mid(i,1) == "."){
	    filetype = " " + filename.mid(i+1,filename.length()-i) + " ";
	}
    }
    // Set highlighting
    if (QString(" cpp cc cxx h hpp hxx ").contains(filetype)) highlighter->loadSyntax("syntax/cpp.syn");
    else if (QString(" latex tex bib ").contains(filetype)) highlighter->loadSyntax("syntax/latex.syn");
    else if (QString(" syn cfg ").contains(filetype)) highlighter->loadSyntax("syntax/coderoom.syn");
    else highlighter->loadSyntax("syntax/text.syn");
}

void Editor::chooseSyntax(){
    // Load data
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"), "", tr("All Files (*)"));
    // Check if filename is empty
    if (filename == "") return;
    // Set syntax
    highlighter->loadSyntax(filename);
    this->strToData(this->dataToStr()); // Refresh
}

void Editor::openFile(){
    // Load data
    QString filename = QFileDialog::getOpenFileName(this,tr("Open File"), "", tr("All Files (*)"));
    // Open file
    this->openFile(filename);
}

void Editor::openFile(QString filename){
    // Check if filename is empty
    if (filename == "") return;
    // Set syntax
    this->setSyntax(filename);
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
    // Set syntax
    this->setSyntax(filename);
    this->strToData(this->dataToStr()); // Refresh
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
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) throw QString("Error opening file");
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
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) throw QString("Error opening file");
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
