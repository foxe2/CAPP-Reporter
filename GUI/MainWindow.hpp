#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "main.hpp"

#include <map>
#include <vector>
#include <QString>
#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QSyntaxHighlighter>


//Forward declarations
class TentativeHighlighter;
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

    //Reset's classesTaken to empty
    void reset();

private:

    //Draw, position, and connect items
    void PositionObjects(int,int);
    void connectDefaults();
    void drawOutlines();

    //Generates course to be added or removed as a string
    //This function returns true if a valid course has been entered
    bool updateCourse();

    //Update the classes taken. This will subsequently
    //update everything else when it finishes. The first
    //determines whether or not this was a tentative update
    //or a non-tentative. If it does not equal 0 then theCourse
    //is highlighted the color specified. The values to be passed
    //in as an argument shoud only be one of the static color uints below.
    void updateClassesTaken(const Qt::GlobalColor Highlight = Qt::black);

    //Used for current courses
    std::map<const QString, const QString*> classesTaken;
    TentativeHighlighter * highlighter;
    QString * theCourse;

    //GUI representation
    Ui::MainWindow *ui;
    const int StrokeThickness;
    std::vector<QGraphicsRectItem*> Outlines;

    //Prevent mulitple GUIs
    static uint GUICount;

};

#endif // MAINWINDOW_HPP
