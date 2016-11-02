#include "main.hpp"

#include <vector>

#include <QGraphicsView>
#include <QGraphicsRectItem>

//Forward declerations
class SmartScene;

//A GUI class
class GUI {
public:

	//Constructor
	GUI();

	//Destructor
	~GUI();

	//Size of the display
    static const uint Width;
    static const uint Height;

private:

    //Draw the outlines of each section
    void drawOutlines();

	//Representation
	SmartScene * theScene;
    QGraphicsView * theView;
    const int StrokeThickness;
    std::vector<QGraphicsRectItem*> Outlines;

    //Prevent mulitple GUIs
    static uint GUICount;
};
