#ifndef COURSESELECTOR_HPP
#define COURSESELECTOR_HPP

#include "main.hpp"

#include <map>

#include <QString>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>

//Forward declarations
class ColorText;

//Helpful typedef
typedef std::map<const QString, const QString*> courses;

//A class to handle the courses taken
//and the graphics associated with them
class CourseSelector : public QObject {
    Q_OBJECT
public:

	//Constructor
    CourseSelector(QComboBox*,QComboBox*,QComboBox*,QTextEdit*,QPushButton*);

	//Destructor
	~CourseSelector();

    //Reset's classesTaken to empty
    void reset();

public slots:

    //Connects or disconnects what is
    //needed when the user decides to
    //activate or deactivate auto-updating
    void tentativeToggle(bool);

    //Tentatively add or remove a class as needed
    void tentativelyAlterClasses(const QString&);

    //Add or remove a class
    void removeClass();
    void addClass();

    //Read classes from a file
    void readFromFile();

    //Get the courses taken for the algo
    const std::map<std::string, int> * getCoursesTaken();

signals:

	//A signal to be emitted if updateClassesTaken is called
    void coursesChanged();

private:

    //Generates course to be added or removed as a string
    //This function returns true if a valid course has been entered
    bool updateCourse();

    //Update the classes taken. This will subsequently
    //update everything else when it finishes. This function
	//will highlight theCourse the color passed as the argument
    void updateClassesTaken(const Qt::GlobalColor);

    //Representation
    bool tentativeToggled;

    //Used for current courses
    ColorText * highlighter;
    courses classesTaken;
    QString * theCourse;

    //Used to store GUI items
    QTextEdit * currentCourses;
    QPushButton * readFromFileBtn;
    const QComboBox * courseMajor;
    const QComboBox * courseNumber;
    const QComboBox * numCredits;

	//Ensure there is only one CourseSelector
    static uint staticCount;
};

#endif 
