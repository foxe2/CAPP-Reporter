#include "main.hpp"

#include <QGraphicsView>

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

	//Representation
	SmartScene * theScene;
	QGraphicsView * theView;

    //Prevent mulitple GUIs
    static uint GUICount;
};
