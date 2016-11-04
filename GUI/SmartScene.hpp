#ifndef SMARTSCENE_H
#define SMARTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

//Forward declarations
class Highlight;

//A custom QGraphicsScene
class SmartScene : public QGraphicsScene {
public:

	//Constructor
	SmartScene();

private slots:

	//If the mouse was clicked
	void mousePressEvent(QGraphicsSceneMouseEvent *);

};

#endif // SMARTSCENE_H
