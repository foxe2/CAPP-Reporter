#include "SmartScene.hpp"
#include "GUI.hpp"

//Constructor
SmartScene::SmartScene() {

    //Set the scene size
    setSceneRect(0,0,GUI::Width,GUI::Height);
}

//If the mouse was clicked, highlight the appropriate square
void SmartScene::mousePressEvent(QGraphicsSceneMouseEvent *e) {}
