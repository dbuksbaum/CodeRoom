#include "textfind.h"

TextFind::TextFind(QWidget *parent) : QWidget(parent){
}

TextFind::~TextFind(){
    delete btnFindNext;
    delete btnReplaceThis;
    delete btnReplaceAll;
    delete txtFind;
    delete txtReplace;
    delete radUp;
    delete radDown;
    delete chkCase;
}
