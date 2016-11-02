#include "GUI.hpp"
#include "SmartScene.hpp"


//Size of the scene
const uint GUI::Width = 800;
const uint GUI::Height = 600;

//Default number of GUIs
uint GUI::GUICount = 0;

//Draw the outlines of each section
void GUI::drawOutlines() {

    //For readability
    const int ST = StrokeThickness/2;

    //Create a specialized pen
    QPen p;
    p.setCosmetic(false);
    p.setWidth(StrokeThickness);

    //Top major selection
    Outlines.push_back(theScene->addRect(ST, ST, Width/2-ST, Height/8-ST, p));

    //Main requirement
    Outlines.push_back(theScene->addRect(ST, Height/8, Width/2-ST, (7*Height)/8-ST, p));

    //Hass requirement
    Outlines.push_back(theScene->addRect(Width/2, ST, Width/4, Height-2*ST, p));

    //Enter courses
    Outlines.push_back(theScene->addRect((3*Width)/4, ST, Width/4-ST, (5*Height)/8-ST, p));

    //Courses entered
    Outlines.push_back(theScene->addRect((3*Width)/4, (5*Height)/8, Width/4-ST, (3*Height)/8-ST, p));
}

//Constructor
GUI::GUI() : StrokeThickness(4) {

    //Make sure only one GUI exists
    Assert(!GUI::GUICount, "only 1 GUI can exist");
    GUI::GUICount = 1;

    //Make the scene and view
    theScene = new SmartScene();
    theView = new QGraphicsView(theScene);
    theView->setFixedSize(Width, Height);

    //Draw outlines
    drawOutlines();

    //Show the GUI
    theView->show();
};

//Destructor
GUI::~GUI() {

    //Prevent memory leaks
    delete theView; delete theScene;
    for (auto i : Outlines) delete i;
}
