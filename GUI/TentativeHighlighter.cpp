#include "TentativeHighlighter.hpp"


//Constructor
TentativeHighlighter::TentativeHighlighter(QTextEdit * parent)
    : QSyntaxHighlighter(parent) {
    theColor = Qt::black;
    highlight = NULL;
}

//Destructor
TentativeHighlighter::~TentativeHighlighter() { delete highlight; }

//Set current highlighting information
void TentativeHighlighter::setHighlightInfo(const QString& h, const Qt::GlobalColor c) {
    highlight = new QString(h);
    theColor = c;
}

//Reset variables
void TentativeHighlighter::reset() {
    delete highlight;
    highlight = NULL;
    theColor = Qt::black;
}

//Override the highlighting function
#include <QDebug>
void TentativeHighlighter::highlightBlock(const QString& text) {

    //If there is no reason to call this function, skip it
    if (!highlight) return;

	//The current line to be read
	QString currentLine;

    qDebug() << "The text is: " << text;
    qDebug() << "Highlight " << highlight << " - " << theColor;
    qDebug() << "|" << *highlight << "|";

    if (text == *highlight) setFormat(0, text.size(), theColor);
    else setFormat(0, text.size(), Qt::black);

    //Call reset before returning
    //reset();
}
