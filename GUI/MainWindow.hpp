#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "main.hpp"

#include <map>
#include <vector>

#include <QMainWindow>
#include <QGraphicsRectItem>

//For ease
namespace Ui { class MainWindow; }

//Forward declerations
class CourseSelector;

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

	//Reset the application
    void reset();

    //Update the GUI
    void updateAll();
    void updateAll(const QString&);

private:

    //Draw, position, and connect items
    //Notes, drawOutliens calls positionObjects
    void positionObjects(int,int,int);
    void connectDefaults();
    void drawOutlines();

    //Inform the user of needed information
    void displayStartupWarning();

	//Used to select courses
    CourseSelector * courses;

    //GUI representation
    Ui::MainWindow * ui;
    const int StrokeThickness;
    std::vector<QGraphicsRectItem*> Outlines;

    //Prevent mulitple GUIs
    static uint GUICount;
};

#endif // MAINWINDOW_HPP
