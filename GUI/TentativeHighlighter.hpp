#include <QSyntaxHighlighter>

//A class used to highlight a line in a QTextEdit
class TentativeHighlighter : public QSyntaxHighlighter {
public:

	//Constructor
	TentativeHighlighter(const QString * h);	

protected:

	//Override the highlighting function
	void highlightBlock(const QString& text);

private:

	//The string and color to highlight
	Qt::GlobalColor theColor;
	QString * highlight;
};
