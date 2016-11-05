#include "MainWindow.hpp"

#include <QApplication>


//Main function
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    //Make the main window
    MainWindow w;

    //Run the program
    w.show(); return a.exec();
}
