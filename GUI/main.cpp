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

//Main function
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	//Make the GUI
	new GUI();

	//Run it
	return a.exec();
}
