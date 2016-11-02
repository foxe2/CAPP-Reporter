#include "main.hpp"
#include "GUI.hpp"

#include <QApplication>


//Prints Msg to stderr and exits
void Err(const char * Msg) {
	fprintf(stderr, "Error, %s\n", Msg);
	exit(EXIT_FAILURE);
}

//Perrors, prints Msg to stderr, and exits
void pErr(const char * Msg) {
	perror(""); Err(Msg);
}

//Asserts a value is true, Err(Msg) if not.
void Assert(const bool b, const char * Msg) { if(!b) Err(Msg); }

#include <QtGui>
//Main function
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	//Make the GUI
    new GUI();

#if 0
    QGraphicsView view;
    QGraphicsScene scene;

    QGraphicsRectItem *item1 = scene.addRect(20, 20, 20, 20);
    QGraphicsRectItem *item2 = scene.addRect(50, 20, 20, 20);

    QPen pen1, pen2;
    pen1.setWidth(5);
    pen2.setWidth(5);
    pen1.setCosmetic(true);
    pen2.setCosmetic(false);

    item1->setPen(pen1);
    item2->setPen(pen2);

    view.setScene(&scene);
    view.scale(4, 4); // both rects are the same size, but one a has wider pen
    view.show();
#endif

	//Run it
	return a.exec();
}
