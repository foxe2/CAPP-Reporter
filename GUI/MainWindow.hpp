#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "main.hpp"

#include <QMainWindow>
#include <QGraphicsRectItem>

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

private:
	//Draw the outlines of each section
	void drawOutlines();


	//Representation
    Ui::MainWindow *ui;
    const int StrokeThickness;
	std::vector<QGraphicsRectItem*> Outlines;

	//Prevent mulitple GUIs
	static uint GUICount;

};

#endif // MAINWINDOW_HPP
