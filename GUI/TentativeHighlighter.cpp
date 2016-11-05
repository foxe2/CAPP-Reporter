#include <QSyntaxHighlighter>

//Constructor
TentativeHighlighter::TentativeHighlighter(const QString * h) : highlight(h), 
	theColor(color==YELLOW ? Qt::yellow : Qt::red) {}

//Override the highlighting function
void TentativeHighlighter::highlightBlock(const QString& text); {

	//The current line to be read
	QString currentLine;

	//For each character in the text
	for(int i = 0; i < text.size(); i++) {

		//If we are not at the newline charcter
		//then add it to the currentLine string
		if (text.at(i) != '\n') currentLine += text.at(i);

		//Otherwise, check if this line should
		//be highlighted. If so, do so then return
		//line should be highlighted
		else if (currentLine == *highlight) {
			setFormat(i-currentLine.size(), currentLine.size()+1, theColor);
			return;
		}
	}
}
