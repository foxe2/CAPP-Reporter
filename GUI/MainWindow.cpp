#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "CourseSelector.hpp"
#include "../Algorithm.hpp"

#include <QMessageBox>
#include <QStringList>

//Size of the scene
const uint MainWindow::Width = 800;
const uint MainWindow::Height = 600;

//Default number of GUIs
uint MainWindow::GUICount = 0;


//--------------------------Constructor/Destructor-----------------------


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
    ui->graphicsView->setScene(new QGraphicsScene(ui->graphicsView));
    ui->graphicsView->scene()->setSceneRect(0,0,MainWindow::Width,MainWindow::Height);

    //Disable window scrolling
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

	//Create CourseSelector
    courses = new CourseSelector(ui->courseMajor, ui->courseNumber, ui->numCredits,
								 ui->currentCourses, ui->readFromFile);

    //Draw and position the GUI's items
    drawOutlines();   

    //Conect everything up
    connectDefaults();

    //Set default numCredits to 4
    ui->numCredits->setCurrentIndex(4-1);

    //Final setup
    reset(); ui->graphicsView->lower();
    setMaximumSize(this->size());
    loadCourseIntoComboBoxes();
    displayStartupWarning();
}

//Destructor
MainWindow::~MainWindow() {

    //Delete courses taken
    delete courses;

    //UI destructor calls destructor
    //of all items it is the parent of
    delete ui;
}


//------------------------------Drawing GUI------------------------------


//Draw the outlines of each section
void MainWindow::drawOutlines() {

    //For organization
    int coursesH = Width/5;
    int mainWidth = Width/2;
    int topHeight = Height/8;

    //Create a specialized pen
    QPen p; p.setCosmetic(false);
    p.setWidth(StrokeThickness);

    //For readability
    const int ST = StrokeThickness/2;
    auto theScene = ui->graphicsView->scene();

    //Top major selection
    Outlines.push_back(theScene->addRect(ST, ST,
        mainWidth-ST, topHeight-ST, p));

    //Main requirement
    Outlines.push_back(theScene->addRect(ST, topHeight,
        mainWidth-ST, Height-topHeight-2*ST, p));

    //Hass requirement
    Outlines.push_back(theScene->addRect(mainWidth, ST,
        mainWidth/2, Height-3*ST, p));

    //Enter courses
    Outlines.push_back(theScene->addRect((3*mainWidth)/2, ST,
        mainWidth/2-2*ST, coursesH-ST, p));

    //Courses entered
    Outlines.push_back(theScene->addRect((3*mainWidth)/2, coursesH,
        mainWidth/2-2*ST, Height-coursesH-2*ST, p));

    //Position the objects in the outlines
    positionObjects(coursesH, mainWidth, topHeight);
}

//Set the positions of everything
void MainWindow::positionObjects(int coursesH, int mainWidth, int topHeight) {

    //Distance from stroke constants
    const int xDFS = 1 + /*The distance ->*/ 5;
    const int yDFS = 1 + /*The distance ->*/ 5;

    //For readability
    int x, y, x1, y1;
    const int ST = StrokeThickness/2;

    //Position the main display
    x = 2*ST + xDFS;
    y = topHeight + yDFS + ST;
    x1 = mainWidth - x - xDFS;
    y1 = Height - y - yDFS - 2*ST;
    ui->mainText->setGeometry(QRect(x,y,x1,y1));

    //Position the hass display
    x = mainWidth + ST + xDFS;
    y = 0 + yDFS + 2*ST;
    x1 = 3*mainWidth/2 - x - xDFS;
    y1 = Height - y - yDFS - 2*ST;
    ui->hassText->setGeometry(QRect(x,y,x1,y1));

    //Position the current courses display
    x = 3*mainWidth/2 + ST + xDFS;
    y = coursesH + ST + yDFS;
    x1 = Width - x - xDFS - 2*ST;
    y1 = Height - y - yDFS - 2*ST /* Account for bottom buttons */ - 25;
    ui->currentCourses->setGeometry(QRect(x,y,x1,y1));

    //Size the graphics view
    ui->graphicsView->setFixedSize(this->size());
    ui->graphicsView->lower();
}


//----------------------Setting up Signals and slots---------------------


//Connect signal and slot defaults
void MainWindow::connectDefaults() {

    //Connect the toggle switch to the toggle slot
    QObject::connect(ui->toggleTentaive, SIGNAL(toggled(bool)),
                     courses, SLOT(tentativeToggle(bool)));

    //Connect add class button to the addClass function
    QObject::connect(ui->addClassButton, SIGNAL(clicked()),
                     courses, SLOT(addClass()));

    //Connect remove class button to the removeClass function
    QObject::connect(ui->removeClassButton, SIGNAL(clicked()),
                     courses, SLOT(removeClass()));

    //Connect read from file button
    QObject::connect(ui->readFromFile, SIGNAL(clicked()),
                     courses, SLOT(readFromFile()));

    //Connect the reset button to the reset slot
    QObject::connect(ui->resetButton, SIGNAL(clicked()),
                     this, SLOT(reset()));

    //Connect courses update function to dependencies
    QObject::connect(courses, SIGNAL(coursesChanged()),
                     this, SLOT(updateAll()));

    //Connect primaryMajor box
    QObject::connect(ui->primaryMajor, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateAll(const QString&)));
    QObject::connect(ui->primaryMajor, SIGNAL(currentTextChanged(QString)),
            this, SLOT(updateAll(const QString&)));

    //Connect secondaryMajor box
    QObject::connect(ui->secondaryMajor, SIGNAL(editTextChanged(QString)),
            this, SLOT(updateAll(const QString&)));
    QObject::connect(ui->secondaryMajor, SIGNAL(currentTextChanged(QString)),
            this, SLOT(updateAll(const QString&)));

    //Set the defaults
    courses->tentativeToggle(true);
}


//----------------------------GUI final setup---------------------------


//Reset the application
void MainWindow::reset() { courses->reset(); }

//Load courses into major ComboBoxes
//This allows the auto-complete to work
void MainWindow::loadCourseIntoComboBoxes() {

    //Make the list
    QStringList tmp = QStringList() << "ARCH" << "ARTS" << "ASTR" << "BCBP"
                                    << "BIOL" << "BMED" << "CHEM" << "CHME"
                                    << "CIVL" << "COGS" << "COMM" << "CSCI"
                                    << "ECON" << "ECSE" << "ENVE" << "ERTH"
                                    << "IHSS" << "ISYE" << "ITWS" << "LANG"
                                    << "LITR" << "MANE" << "MATH" << "MATP"
                                    << "MGMT" << "MTLE" << "PHIL" << "PHYS"
                                    << "PSYC" << "STSH" << "STSS" << "WRIT";

    //Load the list
    ui->courseMajor->addItems(tmp);
    ui->secondaryMajor->addItems(tmp);
}


//-------------------------Altering GUI's output------------------------


//Display a warning to the user on startup
void MainWindow::displayStartupWarning() {

    //Make the warning
    QString txt = tr("CAUTION: Read the READ.ME before using this program. ");
    txt += tr("It contains extremely vital information about the current state ");
    txt += tr("of the program and it's known weaknesses and shortcomings.");

    //Warn the user
    QMessageBox::warning(NULL, QObject::tr(APPLICATION_NAME),
                            txt, QMessageBox::Ok, QMessageBox::Ok);
}

//This function is a slot that simply calls the function below
//It exists because some signals require the dummy argument it has
void MainWindow::updateAll(const QString&) { updateAll(); }

//The function is taken when the GUI's display needs to be
//updated. It is triggered by a signal emitted from CourseSelector
void MainWindow::updateAll() {

    //Create the file name
    std::string fileName = "Database/Majors/";
    fileName += ui->primaryMajor->currentText().toLatin1().constData() + std::string("-");
    fileName += ui->secondaryMajor->currentText().toLatin1().constData();
    fileName.append(".txt");

    //Output text
    QString mainTxt = tr(""), hassTxt = tr("");

    //Run the algorithm and record the output
    auto tmpMap = courses->getCoursesTaken();
    auto inputMap = *tmpMap;
    const auto algoOutput = Algo::runAlgo(fileName, inputMap);

    //Make HASS string
    for(auto i : *(algoOutput.second)) {
        hassTxt.append(tr("•") + tr(i.second.c_str()) + tr("\n"));
        hassTxt.append(tr(i.first.c_str()) + tr("\n\n"));
    }

    //Make Main string
    for(auto i : *(algoOutput.first)) {
        mainTxt.append(tr("•") + tr(i.second.c_str()) + tr("\n"));
        mainTxt.append(tr(i.first.c_str()) + tr("\n\n"));
    }

    //Finalize the output
    if (mainTxt == tr("") && hassTxt == tr(""))
        mainTxt = tr("Error: Major combination doesn't exist or ")
                + tr("has yet to be implemented in this application");
    else {
        hassTxt.prepend(tr("HASS Requirements remaining:\n\n"));
        mainTxt.prepend(tr("Major Requirements remaining:\n\n"));
    }

    //Update the GUI
    ui->hassText->setPlainText(hassTxt);
    ui->mainText->setPlainText(mainTxt);

    //Prevent leaks
    delete algoOutput.second;
    delete algoOutput.first;
    delete tmpMap;
}
