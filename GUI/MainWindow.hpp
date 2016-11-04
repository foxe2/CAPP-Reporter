#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "main.hpp"

#include <QMainWindow>
#include <QGraphicsRectItem>

#include <set>
#include <string>
#include <vector>

//Forward declarations
class SmartScene;

//For ease
namespace Ui { class MainWindow; }

//Main window class
class MainWindow : public QMainWindow {
    Q_OBJECT
public:

	//Constructor
    explicit MainWindow(QWidget *parent = 0);

	//Destructor
    ~MainWindow();

	//Size of the display
	static const uint Width;
	static const uint Height;

private slots:

    //Called to enable or disable
    //tentative class adding/removing
    void tentativeToggle(bool checked);

    //Tentatively add or remove a class
    void tentativelyRemoveClass();
    void tentativelyAddClass();

    //Add or remove a class
    void removeClass();
    void addClass();

private:

    //Draw, position, and connect items
    void PositionObjects(int,int);
    void connectDefaults();
    void drawOutlines();

    //Update the classes taken
    //This will subsequently update
    //everything else when it finishes
    void updateClassesTaken();

    //Representation
    Ui::MainWindow *ui;
    const int StrokeThickness;
    std::string tentativeClassTaken;
    std::set<std::string> classesTaken;
	std::vector<QGraphicsRectItem*> Outlines;

	//Prevent mulitple GUIs
	static uint GUICount;

};

#endif // MAINWINDOW_HPP
