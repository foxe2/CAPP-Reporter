#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "SmartScene.hpp"

//Size of the scene
const uint MainWindow::Width = 800;
const uint MainWindow::Height = 600;

//Default number of GUIs
uint MainWindow::GUICount = 0;

//Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow), StrokeThickness(4) {

	//Make sure only one GUI exists
    Assert(!MainWindow::GUICount, "only 1 GUI can exist");
    MainWindow::GUICount = 1;
	
	//Setup the ui
	ui->setupUi(this);
    ui->centralWidget->setFixedSize(Width, Height);
    ui->graphicsView->setFixedSize(Width, Height);
    ui->graphicsView->setScene(new SmartScene());

    //Disable scrolling
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

	//Draw outlines
    drawOutlines();
}

//Destructor
MainWindow::~MainWindow() {

    //UI destructor calls destructor of all items it has
    delete ui;

#if 0 //TODO: figure out if these must be deleted
    //Record information to delete
    auto theScene = ui->graphicsView->scene();
    //Must delete destructed items still
#endif
}

//Draw the outlines of each section
void MainWindow::drawOutlines() {

    //For organization
    int CoursesH = Width/5;
    int MainWidth = Width/2;
    int TopHeight = Height/8;

    //Create a specialized pen
    QPen p; p.setCosmetic(false);
    p.setWidth(StrokeThickness);

    //For readability
    const int ST = StrokeThickness/2;
    auto theScene = ui->graphicsView->scene();

    //Top major selection
    Outlines.push_back(theScene->addRect(ST, ST, MainWidth-ST, TopHeight-ST, p));

	//Main requirement
    Outlines.push_back(theScene->addRect(ST, TopHeight, MainWidth-ST, Height-TopHeight-ST, p));

	//Hass requirement
    Outlines.push_back(theScene->addRect(MainWidth, ST, MainWidth/2, Height-2*ST, p));

	//Enter courses
    Outlines.push_back(theScene->addRect((3*MainWidth)/2, ST, MainWidth/2-ST, CoursesH-ST, p));

	//Courses entered
    Outlines.push_back(theScene->addRect((3*MainWidth)/2, CoursesH, MainWidth/2-ST, Height-CoursesH-ST, p));
}

