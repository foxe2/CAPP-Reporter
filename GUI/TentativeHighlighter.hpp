#include "main.hpp"

#include <QTextEdit>
#include <QSyntaxHighlighter>

//A class used to highlight a line in a QTextEdit
class TentativeHighlighter : public QSyntaxHighlighter {
public:

	//Constructor
    TentativeHighlighter(QTextEdit * parent);

    //Destructor
    ~TentativeHighlighter();

    //Set current highlighting information this will call reset
    void setHighlightInfo(const QString& h, const Qt::GlobalColor c);

protected:

	//Override the highlighting function
	void highlightBlock(const QString& text);

private:

    //Resets theColor and highlight
    void reset();

	//The string and color to highlight
    const QString * highlight;
    Qt::GlobalColor theColor;

};
