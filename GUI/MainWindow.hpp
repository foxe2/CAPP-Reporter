#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include "main.hpp"

#include <map>
#include <vector>
#include <QString>
#include <QMainWindow>
#include <QGraphicsRectItem>

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

	//Reset the application
    void reset();

private:

    //Draw, position, and connect items
    //Notes, drawOutliens calls positionObjects
    void positionObjects(int,int,int);
    void connectDefaults();
    void drawOutlines();

	//Used to select courses
	courseSelector * courses;

    //GUI representation
    Ui::MainWindow *ui;
    const int StrokeThickness;
    std::vector<QGraphicsRectItem*> Outlines;

    //Prevent mulitple GUIs
    static uint GUICount;
};

#endif // MAINWINDOW_HPP
