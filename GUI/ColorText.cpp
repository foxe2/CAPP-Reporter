#include "ColorText.hpp"

//Constructor
ColorText::ColorText(QTextEdit * parent)
    : QSyntaxHighlighter(parent) {
    theColor = Qt::black;
    highlight = NULL;
}

//Destructor
ColorText::~ColorText() { delete highlight; }

//Set current highlighting information
void ColorText::setHighlightInfo(const QString& h, const Qt::GlobalColor c) {
    highlight = new QString(h);
    theColor = c;
}

//Override the highlighting function
void ColorText::highlightBlock(const QString& text) {

    //If there is no reason to call this function, skip it
    if (!highlight) return;

	//The current line to be read
	QString currentLine;

    //Highlight as needed
    if (text.mid(2,text.size()-2) == *highlight) setFormat(0, text.size(), theColor);
    else setFormat(0, text.size(), Qt::black);
}
