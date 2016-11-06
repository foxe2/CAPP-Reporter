#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "TentativeHighlighter.hpp"

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

    //Initalize variables
    theCourse = new QString("");

    //Setup the ui
    ui->setupUi(this);
    ui->centralWidget->setFixedSize(Width, Height);
    ui->graphicsView->setFixedSize(Width, Height);
    ui->graphicsView->setScene(new QGraphicsScene(ui->graphicsView));
    ui->graphicsView->scene()->setSceneRect(0,0,MainWindow::Width,MainWindow::Height);

    //Disable scrolling
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    //Disable autocomplete for course number box
    ui->courseNumber->setCompleter(NULL);

    //Draw and position the GUI's items
    drawOutlines();

    //Conect everything up
    connectDefaults();

    //Create a custom syntax highlighter
    highlighter = new TentativeHighlighter(ui->currentCourses);

    //Draw the GUI
    updateClassesTaken();
}

//Destructor
MainWindow::~MainWindow() {

    //UI destructor calls destructor
    //of all items it is the parent of
    delete ui;

    //Must delete destructed items still
    for(auto i : classesTaken) delete i.second;

    //Reset the string
    delete theCourse;
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

#include <QDebug>
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
    y1 = Height - y - yDFS - 2*ST;
    ui->currentCourses->setGeometry(QRect(x,y,x1,y1));
}

void MainWindow::connectDefaults() {

    //Connect the toggle switch to the toggle slot
    QObject::connect(ui->toggleTentaive, SIGNAL(toggled(bool)),
                     this, SLOT(tentativeToggle(bool)));

    //Connect add class button to the addClass function
    QObject::connect(ui->addClassButton, SIGNAL(pressed()),
                     this, SLOT(addClass()));

    //Connect remove class button to the removeClass function
    QObject::connect(ui->removeClassButton, SIGNAL(pressed()),
                     this, SLOT(removeClass()));

    //Connect the reset button to the reset slot
    QObject::connect(ui->resetButton, SIGNAL(pressed()),
                     this, SLOT(reset()));

    //Set the defaults
    tentativeToggle(true);
}

//Called to enable or disable
//tentative class adding/removing
void MainWindow::tentativeToggle(bool checked) {

    //Enable auto update
    if (checked) {

        //Connect course Major box
        QObject::connect(ui->courseMajor, SIGNAL(currentTextChanged(QString)),
                         this, SLOT(tentativelyAlterClasses(const QString&)));
        QObject::connect(ui->courseNumber, SIGNAL(currentTextChanged(QString)),
                         this, SLOT(tentativelyAlterClasses(const QString&)));

        //Connect course Number box
        QObject::connect(ui->courseNumber, SIGNAL(currentTextChanged(QString)),
                         this, SLOT(tentativelyAlterClasses(const QString&)));
        QObject::connect(ui->courseNumber, SIGNAL(editTextChanged(QString)),
                         this, SLOT(tentativelyAlterClasses(const QString&)));
    }

    //Disable auto update
    else {

        //Disconnect course Major box
        QObject::disconnect(ui->courseMajor, SIGNAL(currentTextChanged(QString)),
                            this, SLOT(tentativelyAlterClasses(const QString&)));
        QObject::disconnect(ui->courseNumber, SIGNAL(currentTextChanged(QString)),
                            this, SLOT(tentativelyAlterClasses(const QString&)));

        //Disconnect course Number box
        QObject::disconnect(ui->courseNumber, SIGNAL(currentTextChanged(QString)),
                            this, SLOT(tentativelyAlterClasses(const QString&)));
        QObject::disconnect(ui->courseNumber, SIGNAL(editTextChanged(QString)),
                            this, SLOT(tentativelyAlterClasses(const QString&)));
    }
}


//----------------------Setting up Signals and slots---------------------


//Update the definition of theCourse
//Returns true if this course is possible 		TODO: make it check files, numbers, majors, etc
bool MainWindow::updateCourse() {
    delete theCourse; theCourse = new QString(" ");
    theCourse->prepend(ui->courseMajor->currentText());
    theCourse->append(ui->courseNumber->currentText());
    return (ui->courseMajor->currentText().size() == 4) &&
           (ui->courseNumber->currentText().size() == 4);
}


//-----------------------Altering course selection----------------------


//Called if the tentative class selection changed
void MainWindow::tentativelyAlterClasses(const QString&) {

    //Update theCourse
    updateCourse();

    //If the class is new, tentatively add it, update the GUI
    if (classesTaken.find(*theCourse) == classesTaken.end())
        updateClassesTaken(Qt::green);

    //Otherwise, tentatively remove it, update the GUI
    else updateClassesTaken(Qt::red);
}

//Remove a class
void MainWindow::removeClass() {

    //Update theCourse, return if it is invalid
    if (!updateCourse()) return;

    //Remove the class if it exists
    auto tmp = classesTaken.find(*theCourse);
    if (tmp != classesTaken.end()) {
        delete tmp->second;
        classesTaken.erase(tmp);

        //Update the GUI
        updateClassesTaken();
    }
}

//Add a class
void MainWindow::addClass() {

    //Update theCourse, return if it is invalid
    if (!updateCourse()) return;

    //If there is nothing to do, return
    if (classesTaken.find(*theCourse) != classesTaken.end()) return;

    //Add the class and update the GUI
    classesTaken[*theCourse] = new QString(*theCourse);
    updateClassesTaken();
}

//Reset classesTaken
void MainWindow::reset() {

    //Prevent memory leaks
    for(auto i : classesTaken) delete i.second;

    //Reset
    classesTaken.clear();
}


//-------------------------Altering GUI's output------------------------


//Update the GUI's classes take list, and update the rest subsequently
void MainWindow::updateClassesTaken(const Qt::GlobalColor highlightColor) {

    //The string to print
    QString * toPrint = new QString("");

    //Set to true if we are highlighting nothing
    bool printedTentative = (highlightColor == Qt::black);

    //For each class taken
    for(auto i : classesTaken) {

        //If have yet to add theCourse
        if (!printedTentative) {

            //If we should add theCourse
            //here do so and note that we did so
            if (*theCourse < i.first) {
                printedTentative = true;
                *toPrint += QString("+ ") + *theCourse + QString('\n');
            }

            //If we should mark this course to be
            //removed here, do so and note that we did so
            else if (*theCourse == i.first) {
                *toPrint += QString("- ");
                printedTentative = true;
            }
        }

        //Add i to the string
        toPrint->append(i.first); *toPrint += '\n';
    }

    //In case theCourse comes last
    if (!printedTentative) *toPrint += QString("+ ") + *theCourse + QString('\n');

    //Highlight what was requested
    auto tmp = *theCourse;
    highlighter->setHighlightInfo(tmp, highlightColor);

    //Print the string
    ui->currentCourses->setPlainText(*toPrint);

    //TODO: CALL OTHER FUNCTIONS HERE
}
