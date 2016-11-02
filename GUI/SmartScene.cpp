#include "Game.hpp"
#include "SmartScene.hpp"
#include "Highlight.hpp"

//Constructor
SmartScene::SmartScene() {

    //Set the scene size
    setSceneRect(0,0,Game::Width,Game::Height);
    //TODO: Make resizable later

}

//If the mouse was clicked, highlight the appropriate square
void SmartScene::mousePressEvent(QGraphicsSceneMouseEvent *e) {}
