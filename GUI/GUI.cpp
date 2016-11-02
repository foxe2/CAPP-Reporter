#include "GUI.hpp"
#include "SmartScene.hpp"


//Size of the scene
const uint GUI::Width = 800;
const uint GUI::Height = 600;

//Default number of GUIs
uint GUI::GUICount = 0;

//Constructor
GUI::GUI() {

    //Make sure only one GUI exists
    Assert(!GUI::GUICount, "only 1 GUI can exist");
    GUI::GUICount = 1;

	//Make the scene and view
	theScene = new SmartScene();
	theView = new QGraphicsView(theScene);
	theView->setFixedSize(Width, Height);

	//Show the GUI
	theView->show();
};
