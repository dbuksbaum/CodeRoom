#include <QtGui/QApplication>
#include <QStyle>
#include "editor.h"

// Main function
int main(int argc, char *argv[])
{
    QApplication codeRoom(argc, argv);
    // Run editor
    Editor edit;
    edit.showFullScreen();

    return codeRoom.exec();
}
