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
    //editor.showMaximized();

    return codeRoom.exec();
}
