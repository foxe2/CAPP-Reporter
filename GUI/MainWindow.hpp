#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "main.hpp"

#include <QString>
#include <QMainWindow>
#include <QGraphicsRectItem>

#include <map>
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

    //Tentatively add or remove a class as needed
    void tentativelyAlterClasses(const QString&);

    //Add or remove a class
    void removeClass();
    void addClass();

private:

    //Draw, position, and connect items
    void PositionObjects(int,int);
    void connectDefaults();
    void drawOutlines();

    //Generates course to be added or removed as a string
    void updateCourse();

    //Update the classes taken. This will subsequently
    //update everything else when it finishes. The first
    //determines whether or not this was a tentative update
    //or a non-tentative. If it does not equal 0 then theCourse
    //is highlighted the color specified. The values to be passed
    //in as an argument shoud only be one of the static color uints below.
    void updateClassesTaken(const uint Highlight = 0);

    //Representation
    Ui::MainWindow *ui;
    QString * theCourse;
    const int StrokeThickness;
    std::map<const QString, const QString*> classesTaken;
    std::vector<QGraphicsRectItem*> Outlines;

	//Prevent mulitple GUIs
	static uint GUICount;

    //Static color vairables to be used as an enum
    static const uint YELLOW;
    static const uint RED;

};

#endif // MAINWINDOW_HPP
