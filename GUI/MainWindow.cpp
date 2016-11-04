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

    //Note, ui destructor calls delete on every
    //item it has! No need to clean up anything else
    delete ui;
}

//Draw the outlines of each section
void MainWindow::drawOutlines() {

	//For readability
	const int ST = StrokeThickness/2;

	//Create a specialized pen
	QPen p;
	p.setCosmetic(false);
	p.setWidth(StrokeThickness);

    //For readability
    auto theScene = ui->graphicsView->scene();

	//Top major selection
	Outlines.push_back(theScene->addRect(ST, ST, Width/2-ST, Height/8-ST, p));

	//Main requirement
	Outlines.push_back(theScene->addRect(ST, Height/8, Width/2-ST, (7*Height)/8-ST, p));

	//Hass requirement
	Outlines.push_back(theScene->addRect(Width/2, ST, Width/4, Height-2*ST, p));

	//Enter courses
	Outlines.push_back(theScene->addRect((3*Width)/4, ST, Width/4-ST, (5*Height)/8-ST, p));

	//Courses entered
	Outlines.push_back(theScene->addRect((3*Width)/4, (5*Height)/8, Width/4-ST, (3*Height)/8-ST, p));
}

