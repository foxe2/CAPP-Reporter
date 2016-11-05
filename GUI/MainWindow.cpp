#include "MainWindow.hpp"
#include "ui_mainwindow.h"
#include "TentativeHighlighter.hpp"

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

    //Initalize variables
    theCourse = new QString("");

    //Setup the ui
    ui->setupUi(this);
    ui->centralWidget->setFixedSize(Width, Height);
    ui->graphicsView->setFixedSize(Width, Height);
    ui->graphicsView->setScene(new QGraphicsScene());
    ui->graphicsView->scene()->setSceneRect(0,0,MainWindow::Width,MainWindow::Height);

    //Disable scrolling
    ui->graphicsView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
    ui->graphicsView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );

    //Disable autocomplete for course number box
    ui->CourseNumber->setCompleter(NULL);

    //Draw outlines
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

    //UI destructor calls destructor of all items it has
    delete ui;

#if 0 //TODO: figure out if these must be deleted
    //Record information to delete
    auto theScene = ui->graphicsView->scene();
    //Must delete destructed items still
    //Also must delete theCourse and classesTaken[i]
    //delete higlight too?
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

    //Set the defaults
    tentativeToggle(true);
}

//Set the positions of everything
//void PositionObjects(int,int); //TODO: Implement

//Called to enable or disable
//tentative class adding/removing
void MainWindow::tentativeToggle(bool checked) {

    //Enable auto update
    if (checked) {

        //Connect course Major box
        QObject::connect(ui->CourseMajor, SIGNAL(currentTextChanged(QString)),
                         this, SLOT(tentativelyAlterClasses(QString)));
        QObject::connect(ui->CourseNumber, SIGNAL(currentTextChanged(QString)),
                         this, SLOT(tentativelyAlterClasses(QString)));

        //Connect course Number box
        QObject::connect(ui->CourseNumber, SIGNAL(currentTextChanged(QString)),
                         this, SLOT(tentativelyAlterClasses(QString)));
        QObject::connect(ui->CourseNumber, SIGNAL(editTextChanged(QString)),
                         this, SLOT(tentativelyAlterClasses(QString)));
    }

    //Disable auto update
    else {

        //Disconnect course Major box
        QObject::disconnect(ui->CourseMajor, SIGNAL(currentTextChanged(QString)),
                            this, SLOT(tentativelyAddClass(QString)));

        QObject::disconnect(ui->CourseNumber, SIGNAL(currentTextChanged(QString)),
                            this, SLOT(tentativelyAddClass(QString)));

        //Disconnect course Number box
        QObject::disconnect(ui->CourseNumber, SIGNAL(currentTextChanged(QString)),
                            this, SLOT(tentativelyAddClass(QString)));
        QObject::disconnect(ui->CourseNumber, SIGNAL(editTextChanged(QString)),
                            this, SLOT(tentativelyAddClass(QString)));
    }
}

//Update the definition of theCourse
void MainWindow::updateCourse() {
    delete theCourse; theCourse = new QString(" ");
    theCourse->prepend(ui->CourseMajor->currentText());
    theCourse->append(ui->CourseNumber->currentText());
}


#include <QDebug>



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

//Add or remove a class
void MainWindow::removeClass() {

    //Update theCourse
    updateCourse();

    //Remove the class if it exists
    auto tmp = classesTaken.find(*theCourse);
    if (tmp != classesTaken.end()) {
        delete tmp->second;
        classesTaken.erase(tmp);
    }

    //Update the GUI
    updateClassesTaken();
}

void MainWindow::addClass() {

    //Update theCourse
    updateCourse();
    classesTaken[*theCourse] = new QString(*theCourse);
    updateClassesTaken();
}

//Update the GUI's classes take list, and update the rest subsequently
void MainWindow::updateClassesTaken(const Qt::GlobalColor highlightColor) {

    //The string to print
    QString * toPrint = new QString("");

    //Update course if needed
    if (highlightColor != Qt::black) updateCourse();

    //Set to false only if we need to add a course
    bool printedTentative = (highlightColor != Qt::green);

    //For each class taken
    for(auto i : classesTaken) {

        //If have yet to add theCourse
        if (!printedTentative)

            //If this is the correct place to add theCourse
            if (*theCourse < i.first) {
                qDebug() << "- " << (*theCourse) << " >= " << i.first;
                qDebug() << "len(tC) = " << theCourse->size() << ", and len(i) = " << i.first.size();
                toPrint->append(*theCourse); *toPrint += '\n';
                printedTentative = true;
            }

        //Add i to the string
        toPrint->append(i.first); *toPrint += '\n';
    }

    //In case theCourse comes last
    if (!printedTentative) {
        toPrint->append(*theCourse); *toPrint += '\n';
    }

    //Highlight what was requested
    auto tmp = *theCourse;
    highlighter->setHighlightInfo(tmp, highlightColor);

    //Print the string
    ui->currentCourses->setPlainText(*toPrint);

    qDebug() << "Done!\n\n\n";
    //TODO: CALL OTHER FUNCTIONS HERE

}
