#include <QtGui/QApplication>
#include <QStyle>
#include "editor.h"

// Main function
int main(int argc, char *argv[])
{
    QApplication codeRoom(argc, argv);
    // Run editor
    Editor editor;
    editor.showFullScreen();
    // Open file with
    if (argc == 2){
	// Get filename
	QString filename(argv[1]);
	// Open file
	editor.openFile(filename);
    }
    // Run program
    return codeRoom.exec();
}
