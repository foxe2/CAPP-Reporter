#include "CourseSelector.hpp"
#include "ColorText.hpp"

#include <fstream>
#include <sstream>
#include <libgen.h>

#include <QRegExp>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>


//Default number of CourseSelectors
uint CourseSelector::staticCount = 0;


//Constructor
CourseSelector::CourseSelector(QComboBox *a, QComboBox *b, QComboBox *c,
    QTextEdit *d, QPushButton *e) : currentCourses(d), readFromFileBtn(e),
    courseMajor(a), courseNumber(b), numCredits(c) {

		//Make sure only one CourseSelector exists
		Assert(!CourseSelector::staticCount, "only 1 CourseSelector can exist");
		CourseSelector::staticCount = 1;

		//Create a custom syntax highlighter
		highlighter = new ColorText(currentCourses);

		//Initalize theCourse
		theCourse = new QString(tr(""));
	}

//Destructor
CourseSelector::~CourseSelector() {

	//Must delete destructed items still
	for(auto i : classesTaken) delete i.second;

	//Prevent memory leaks
	delete highlighter;
	delete theCourse;
}

//--------------------------------Inlines--------------------------------


//Returns true if the course is legal, false otherwise
inline bool verifyCourse(const QString m, const QString n) {
    QRegExp re("\\d*");
    if (!re.exactMatch((n))) return false;
	return m.size() == 4 && n.size() == 4;
}

//Displays an error message, prompts to retry, then returns result
//If the second argument is passed as false, the function will not display the retry button
inline bool errorPrompt(const QString& txt, bool retry = true) {

	//Display error message txt
    auto buttons = retry ? (QMessageBox::Ok | QMessageBox::Retry) : QMessageBox::Ok;
    int ret = QMessageBox::critical(NULL, QObject::tr(APPLICATION_NAME),
                                    txt, buttons, QMessageBox::Ok);

	//If the user wishes to retry, do so.
	return (ret == QMessageBox::Retry);
}


//----------------------Setting up Signals and slots---------------------


//Called to enable or disable
//tentative class adding/removing
void CourseSelector::tentativeToggle(bool checked) {

	//Enable auto update
	if (checked) {

		//Connect course Major box
		QObject::connect(courseMajor, SIGNAL(editTextChanged(QString)),
				this, SLOT(tentativelyAlterClasses(const QString&)));
		QObject::connect(courseMajor, SIGNAL(currentTextChanged(QString)),
				this, SLOT(tentativelyAlterClasses(const QString&)));

        //Connect course Number box
        QObject::connect(courseNumber, SIGNAL(editTextChanged(QString)),
                this, SLOT(tentativelyAlterClasses(const QString&)));
        QObject::connect(courseNumber, SIGNAL(currentTextChanged(QString)),
                this, SLOT(tentativelyAlterClasses(const QString&)));

        //Connect num Credits Number box
        QObject::connect(numCredits, SIGNAL(editTextChanged(QString)),
				this, SLOT(tentativelyAlterClasses(const QString&)));
        QObject::connect(numCredits, SIGNAL(currentTextChanged(QString)),
				this, SLOT(tentativelyAlterClasses(const QString&)));
	}

	//Disable auto update
	else {

		//Disconnect course Major box
		QObject::disconnect(courseMajor, SIGNAL(editTextChanged(QString)),
				this, SLOT(tentativelyAlterClasses(const QString&)));
		QObject::disconnect(courseMajor, SIGNAL(currentTextChanged(QString)),
				this, SLOT(tentativelyAlterClasses(const QString&)));

		//Disconnect course Number box
		QObject::disconnect(courseNumber, SIGNAL(editTextChanged(QString)),
				this, SLOT(tentativelyAlterClasses(const QString&)));
		QObject::disconnect(courseNumber, SIGNAL(currentTextChanged(QString)),
				this, SLOT(tentativelyAlterClasses(const QString&)));

        //Disconnect num Credits Number box
        QObject::disconnect(numCredits, SIGNAL(editTextChanged(QString)),
                this, SLOT(tentativelyAlterClasses(const QString&)));
        QObject::disconnect(numCredits, SIGNAL(currentTextChanged(QString)),
                this, SLOT(tentativelyAlterClasses(const QString&)));

		//Redraw the GUI
		updateClassesTaken(Qt::black);
	}
}


//-----------------------Altering course selection----------------------


//Format the the pieces into a course string
inline QString * formatCourse(const QString mjr,
                              const QString num,
                              const QString crd) {
    QString * ret = new QString(QObject::tr(" "));
    ret->prepend(mjr);
    ret->append(num);
    ret->append(QObject::tr(" - ") + crd + QObject::tr(" credits"));
    return ret;
}

//Update the definition of theCourse
//Returns true if this course is possible
bool CourseSelector::updateCourse() {
    delete theCourse;
    theCourse = formatCourse(courseMajor->currentText(),
                             courseNumber->currentText(),
                             numCredits->currentText());
	return verifyCourse(courseMajor->currentText(),
			courseNumber->currentText());
}

//Called if the tentative class selection changed
void CourseSelector::tentativelyAlterClasses(const QString&) {

    //Update theCourse. Set color to green if the
    //course if valid. Otherwise set color to yellow
    auto color = updateCourse() ? Qt::green : Qt::darkYellow;

	//If the class is new, tentatively add it, update the GUI
	if (classesTaken.find(*theCourse) == classesTaken.end())
        updateClassesTaken(color);

	//Otherwise, tentatively remove it, update the GUI
	else updateClassesTaken(Qt::red);
}

//Remove a class
void CourseSelector::removeClass() {

	//Update theCourse, if invalid display the error then exit. 
	if (!updateCourse()) {
        QString txt = tr("The following course is invalid.\n") + *theCourse;
		errorPrompt(txt, false);
		return;
	}

	//Remove the class if it exists
	auto tmp = classesTaken.find(*theCourse);
	if (tmp != classesTaken.end()) {
		delete tmp->second;
		classesTaken.erase(tmp);

		//Update the GUI
		updateClassesTaken(Qt::black);
	}
}

//Add a class
void CourseSelector::addClass() {

	//Update theCourse, if invalid display the error then exit. 
	if (!updateCourse()) {
        QString txt = tr("The following course is invalid.\n") + *theCourse;
		errorPrompt(txt, false);
		return;
	}

	//If there is nothing to do, return
	if (classesTaken.find(*theCourse) != classesTaken.end()) return;

	//Add the class and update the GUI
	classesTaken[*theCourse] = new QString(*theCourse);
	updateClassesTaken(Qt::black);
}

//Clear the map passed in, preventing memory leaks
inline void clearMap(courses& a) {
	for(auto i : a) delete i.second;
	a.clear();
}

//Reset classesTaken
void CourseSelector::reset() {
	clearMap(classesTaken);
	updateClassesTaken(Qt::black);
}

//Returns true if the string contains
//solely whitespace or is empty, false otherwise
inline bool emptyString(const std::string& s) {
	for (uint i = 0; i < s.size(); i++)
		if (!isspace(s[i])) return false;
	return !s.size();
}

//If the user wishes to input classes via a file
void CourseSelector::readFromFile() {

	//Prevent clicking readFromFile while choosing a file
	readFromFileBtn->setEnabled(false);

	//Have the user choose a file to read from
	QString inFileName = QFileDialog::getOpenFileName(NULL, tr("Choose the file to read from:"),
			QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
			tr("All files (*);;Text File (*.txt);;Simple Text File (*.stf)"));

	//Re-enable the button
	readFromFileBtn->setEnabled(true);

    //Do nothing if the user clicked cancel
	if (inFileName == tr("")) return;

	//Open the file if possible
	std::ifstream inFile(inFileName.toLatin1().constData());

	//If the file failed to open
	if (inFile.fail()) {

		//Make an error message
		QString txt = tr("The file \"");
		QByteArray tmp = inFileName.toLatin1();
		txt += tr(basename(tmp.data())) + tr("\" failed to open");

		//Error, possibly retry, then exit the function
		if (errorPrompt(txt)) readFromFile();
		return;
	}

	//Create a temporary map to hold the file contents
	//The reason we do this is in case there is an error
	//in reading in the file contents, classesTaken isn't affected
    courses tmpCourses;

	//Set to true if there was an error
	QString failInfo = tr("");

    //Read in each course
    int cNum, credits;
    std::string line, course;
	while (std::getline(inFile, line)) {

		//Ignore whitespace lines
		if (emptyString(line)) continue;

		//Prep to parse the line
		std::istringstream nextCourse(line);

		//If there was an error parsing, note so
        if (!(nextCourse >> course >>cNum >> credits)) {
			failInfo = tr("There was an error reading in file. The");
			failInfo += tr("following line is not correctly formatted:\n");
        }

		//If the course is not valid, note so
        else if (!verifyCourse(QString(course.c_str()), QString::number(cNum)))
			failInfo = tr("The following course is not a valid course:\n");

		//If there was an error, prevent leaks, finish
		//the failInfo message, throw the error, then exit
		if (failInfo != tr("")) {
			clearMap(tmpCourses);
			failInfo += (line.size()<20)?tr(line.c_str()):tr(line.substr(0,20).c_str())+tr("...");
			if (errorPrompt(failInfo)) readFromFile();
			return;
		}

		//If the course is valid, add it to the map
        QString * tmp = formatCourse(QString(course.c_str()),
                                     QString::number(cNum),
                                     QString::number(credits));
		tmpCourses[*tmp] = tmp;
	}

	//Clear classes taken
	reset();

	//Populate classes taken with the new classes
	for(auto i : tmpCourses) classesTaken[i.first] = i.second;
	updateClassesTaken(Qt::black);
}

//Takes in a course and converts it to what the algo expects
inline std::pair<const std::string,const int>
courseParser(const std::string in) {

    //Parse the string
    std::string a,b,k; int c;
    std::stringstream str(in);
    str >> a >> b >> k >> c;

    //Return the pair requested efficiently
    return std::move(make_pair(a+std::string("-")+b, c));
}

//Returns the courses taken
const std::map<std::string, int> * CourseSelector::getCoursesTaken() {

    //Return map
    auto ret = new std::map<std::string, int>();

    //Add each course taken
    for(auto i : classesTaken)
        ret->insert(courseParser(i.first.toLatin1().constData()));

    //Add course tentatively taken
    if (updateCourse())
		ret->insert(courseParser(theCourse->toLatin1().constData()));

    //Return the map without need for dynamic allocation
    return ret;
}


//-------------------------Altering GUI's output------------------------


//Update the GUI's classes taken list, and update dependencies subsequently
void CourseSelector::updateClassesTaken(const Qt::GlobalColor highlightColor) {

	//The string to print
	QString * toPrint = new QString(tr(""));

	//Set to true if we are highlighting nothing
	bool printedTentative = (highlightColor == Qt::black);

	//For each class taken
	for(auto i : classesTaken) {

		//If highlighting was requested 
		if (!printedTentative) {

			//If we should add theCourse
			//here do so and note that we did so
			if (*theCourse < i.first) {
				printedTentative = true;
				*toPrint += QString(tr("+ ")) + *theCourse + QString(tr("\n"));
			}

			//If we should mark this course to be
			//removed here, do so and note that we did so
			else if (*theCourse == i.first) {
				*toPrint += QString(tr("- "));
				printedTentative = true;
			}
		}

		//Add the next class to the string
		toPrint->append(i.first); *toPrint += '\n';
	}

	//In case theCourse comes last
	if (!printedTentative) *toPrint += QString(tr("+ ")) + *theCourse + QString(tr("\n"));

	//Highlight what was requested
	highlighter->setHighlightInfo(*theCourse, highlightColor);

	//Print the string
	currentCourses->setPlainText(*toPrint);

	//Alert the application the courses were changed 
	emit coursesChanged();
}
