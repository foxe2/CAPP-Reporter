#include "Algorithm.hpp"


//For ease of reading
using namespace std;

//Namespace
namespace Algo {

//Takes in a data structure which it will add
//classes to. This function returns false should it fail
bool parseReqs(const string& fName, reqsVector& majorReqs,
		commentVector& majorComments, reqsVector& hassReqs, 
		commentVector& hassComments){

	//Open the file
    ifstream inFile(fName);

	//If there was no error
	if (inFile.is_open()){

		//Initalize variables   
		bool inHass = false;
		string line, lastComment = "";

		//For each line in the requirements file
		while(getline(inFile,line)){

			//vector of strings any of which satisfy the requirement a || b || c -> [a,b,c]
			vector<string> options;

			//If we finished parsing the main requirements, switch sections
            if((int)line.find("// HASS Requirement") != -1){
				inHass = true;
				continue;
			}

			//If the line is a comment then update
			int commentIndex = line.find("//");
            if(commentIndex != -1){
				lastComment = line.substr(commentIndex+2);
				continue;
			}

			//Loop through and parse
			for(int a = line.find("||"); a != -1; a = line.find("||")) {

				//Takes the next course 
				string course = line.substr(0,a);

				//If it cant be taken to satisfy req
				//Insert it to the front of the vector so it can be processed first later
				if(course.substr(0,1).compare("!") == 0)
					options.insert(options.begin(), course);

				else options.push_back(course);

				line=line.substr(a+2);
			}
			//Parse last string section
			if(line.substr(0,1).compare("!") == 0)
				options.insert(options.begin(),line);

			else options.push_back(line);
			//If it is in the HASS section add it to hass unless it is a free elective
			//They are proccessed under major
            if(inHass && (int)options[0].find("$") == -1){
				hassReqs.push_back(options);
				hassComments.push_back(lastComment);
			}
			else{
				majorReqs.push_back(options);
				majorComments.push_back(lastComment);
			}
		}

        //Close the file and return true
        inFile.close();
        return true;
	}

    //If the file failed to open, note so and return false
    else cout << "Could not open " << fName << endl;
    return false;
}

//Takes in a datastructure which it will add classes to
//Used specifically for parsing the concentration files
//Very similar to parseReqs()
void parseConcentrationReqs(vector<string>& reqs, const string& fName){
	cerr << fName << endl;
	//Open the file
	ifstream inFile(fName);

	//If the file opened successfully
	if (inFile.is_open()){

		//Initalize variables
		string line = "";

		//For each line in the file
		while(getline(inFile,line)){
			//vector of strings any of which satisfy the requirement a || b || c -> [a,b,c]
            

			//If it is a comment then ignore it 
            if( (int)line.find("//") != -1) continue;

			//If it cannot be taken to satisy a req then add to the beginning
			if(line.substr(0,1).compare("!") == 0)
				reqs.insert(reqs.begin(),line);

			else reqs.push_back(line);
		}

		//Close the file
		inFile.close();
	}

	//If the file failed to open, note so
	else cout << "Could not open " << fName << endl;
}

//Takes a class range such as CSCI-4000+ and checks to see if any class satisfies that requirement
//Accounts for any classes that do not count in unacceptable
bool specialCompare(const string& req, courseMap& classes, const set<string>& unacceptable, bool noRepeat){

    //Iterate through set checking for a match
	for(courseMap::iterator itr = classes.begin(); 
			itr != classes.end(); ++itr){

        int courseNum = stoi(itr->first.substr(5,4));
		int reqLvl = stoi(req.substr(5,4));

        //If the course subjects match (ex CSCI) and the course number
		//is greater than the required level and it is not unacceptable
		if(itr->first.substr(0,4).compare(req.substr(0,4)) == 0 && 
				unacceptable.find(itr->first) == unacceptable.end() &&
				courseNum >= reqLvl) {

            //Erases the class if it cannot be double counted
			if(noRepeat) classes.erase(itr);

			return true;
		}
	}

	return false;
}

int freeElectives(courseMap& classes, const string& credits){

	int currentCreds = 0, numCreds = stoi(credits);
	if(numCreds==0) return 0;
	for(courseMap::iterator itr = classes.begin(); itr != classes.end(); ++itr){

		currentCreds += itr->second;

		if(currentCreds >= numCreds) return 0;
	}

	return numCreds - currentCreds;
}

bool concentrationCompare(const string& initConcentration, courseMap& classes) {

	//Local variables
	string concentration, cFileName;
	bool noRepeat = true;
	int numCourses;

	//A requirement data structure of all the requirements for a certain concentration file
	vector<string> concReqs;

	//Checks if repition in multiple requirements is allowed (ex Comm intensive)    
	concentration = initConcentration;
	if(concentration.substr(0,1).compare("#") == 0){
		concentration = concentration.substr(1);
		noRepeat = false;
	}

	//The number of classes to be taken in a concentrationCompare
	numCourses = stoi(concentration.substr(0,1));
	cerr << concentration <<endl;
	cFileName = "Database/Concentrations/" + concentration.substr(2) + ".txt";

	//Parse the concentration file for requirements
	parseConcentrationReqs(concReqs, cFileName);
	cerr<< "CONC" << endl;

	//Compare the classes to the concetration requirements
	return compareConcCourses(concReqs, classes, noRepeat, numCourses);
}

//Compares courses for the concentration requirements and
//determines which ones are satisfied
bool compareConcCourses(vector<string>& reqs, courseMap& classes, bool noRepeat, int numCourses) {

	//Determines the first index without an exclusion (ex. !CSCI-4020)
	int before = 0;

	//Excluded courses
	set<string> unacceptable;
	
	//Courses to be erased if the user has numCourses in the concentration
	set<string> taken;

	//Find unacceptable course and increment before   
	while(reqs[before].substr(0,1).compare("!") == 0){
		unacceptable.insert(reqs[before].substr(1));
		before++;
	}

	//Iterates through the rest of the list for valid classes
    for(int i = before; i < (int)reqs.size(); ++i){

    	//If a specific req was satisfied (not the entire concentration)
    	bool satisfied = false;

    	//Current course being evaluated
		string temp;

		//The requirement being evaluated
		string element = reqs[i];

		//Removes parenthesis in the case of (a&&b) 
		if((int)element.find("(") != -1)
			element = element.substr(1,element.length()-2);

		int index;
		while(true){
			
			index = element.find("&&");

			if(index == -1) temp = element;

			else{
				temp = element.substr(0,index);
				element = element.substr(index+2);
			}

			//Range(CSCI-4000+)
			if(temp.substr(0,1).compare("@") == 0) {
				if(concentrationCompare(temp.substr(1), classes)) 
					satisfied = true;
			}

			else if(temp.find("+") != -1)
                satisfied = specialCompare(temp, classes, unacceptable, noRepeat);

			//Regular courses
			else{
				courseMap::iterator itr = classes.find(temp);
				if( itr != classes.end() && taken.find(itr->first) == taken.end()){
					satisfied = true;
					taken.insert(itr->first);
				}
			}

			if(!satisfied || index == -1) break;
		}

		if(satisfied){
			--numCourses;
			--i;
		}

		if (numCourses == 0){
			for(set<string>::iterator itr = taken.begin(); itr != taken.end(); ++itr)
				classes.erase(*itr);
			break;
		}
	}
	return numCourses == 0;
}


void compareCourses(reqsVector& reqs, courseMap& classes, vector<int>& needed) {

    for(int i = 0; i < (int)reqs.size(); ++i){
    	cerr << reqs[i].size() << endl;
		set<string> unacceptable;
		bool satisfied = false;
		int before = 0;

        //Find unacceptable course
		while(reqs[i][before].substr(0,1).compare("!") == 0){
			unacceptable.insert(reqs[i][before].substr(1));
			before++;
		}

        for(int j = before; j < (int)reqs[i].size(); ++j){
        	cerr << reqs[i][j] << endl;
			int index; 
			string temp, element = reqs[i][j];

			if(element.find("$") == 0){

				string credits = element.substr(1);
				int dif = freeElectives(classes, credits);

				if(dif > 0){
					vector<string> creds;
					creds.push_back("You need " + to_string(dif) + " more free elective credits.");
					reqs[i] = creds;
				}

				else satisfied = true;
			}

            else if((int)element.find("(") != -1)
				element = element.substr(1).substr(0,element.length()-2);

            while(true){

				satisfied = false;
				index = element.find("&&");

                if(index == -1) temp = element;

				else{

					temp = element.substr(0,index);
					element = element.substr(index+2);
				}

				//Range(CSCI-4000+)
				if(temp.substr(0,1).compare("@") == 0) {

                    if(concentrationCompare(temp.substr(1), classes))
						satisfied = true;
				}

				else if(temp.find("+") != -1)
                    satisfied = specialCompare(temp, classes, unacceptable, true);

				//Regular courses
				else if(classes.find(temp) != classes.end()){

					satisfied = true;
					classes.erase(reqs[i][j]);
				}

                if(!satisfied || index == -1) break;
			}
			cerr << "here" << endl;
            if(satisfied) break;
		}

        if(!satisfied) needed.push_back(i);
	}
}

//This is called by the GUI.
//Returns a pair of maps where each map contains key-value pairs of an
//unsatisfied requirement and a comment about the requirement. The first element 
//in the pair is a map with Major requirements in it, while the second is for HASS
pair<outputMap*, outputMap*> runAlgo(const string& req_file, courseMap& courses){

	//Initializes all hass data structures
	outputMap* hass = new map<string, string>();
	commentVector hassComments;
	vector<int> hassNeeded;
	reqsVector hassReqs;

	//Initializes all major data structures
	outputMap* major = new map<string, string>();
	commentVector majorComments;
	vector<int> majorNeeded;
	reqsVector majorReqs;

	//Parses the requirements file and puts the
	//requirements and comments in their corresponding data structures
    //If the requirements file doesn't exist, then do nothing, simply return
    if (!parseReqs(req_file, majorReqs, majorComments, hassReqs, hassComments))
        return make_pair(major,hass);

	//Compares the necessary requirements for hass with the user's
	//completed classes and fills the missing requirements in hassNeeded
	compareCourses(hassReqs, courses, hassNeeded);

    //Assigns each missing req with its comment in the hass map
    for(int i = 0; i < (int)hassNeeded.size(); ++i){

		string temp = "";
		vector<string> components = hassReqs[hassNeeded[i]];

        for(int j = 0; j < (int)components.size(); ++j) temp = temp + components[j] + " ";

		(*hass)[temp.substr(0,temp.length()-1)]= hassComments[hassNeeded[i]];

	}

    //Compares the requirements for major
	compareCourses(majorReqs, courses, majorNeeded);

    //Generates major map
    for(int i = 0; i < (int)majorNeeded.size(); ++i){

		string temp = "";
		vector<string> components = majorReqs[majorNeeded[i]];

        for(int j = 0; j < (int)components.size(); ++j) temp = temp + components[j] + " ";

		(*major)[temp.substr(0,temp.length()-1)] = majorComments[majorNeeded[i]];
	}

	//Returns the pair of maps.
	return make_pair(major,hass);
}

#if 0
//Read classes is only used in main() for modular testing
//Reads in a text file with one course per line
//Users enter classes in the GUI and this is not present in a deployment
void readClasses(courseMap& classes, string& fname){

	//Local variables
	string temp;
	int index;

	ifstream inFile(fname);

	if (inFile.is_open()){

		while(getline(inFile,temp)){

			//If comment then skip
            if((int)temp.find("//") != -1) continue;

			//Adds each class with the course name as
			//the key and number of credits as the value
			index = temp.find(":");
			classes[temp.substr(0, index)] = stoi(temp.substr(index+1));
		}

		inFile.close();
	}

	else cout << "error";
}

//Only used for modular testing and is called solely in main()
//Outputs the data to a file 
void fileOutput(string& fName, pair<outputMap*, outputMap* > pairMH){

	ofstream outFile(fName);

	if (!outFile.is_open()) cout << "Error writing to output file";

	outFile << "CAUTION: Read the READ.ME before using this program." << endl;
	outFile << "It contains extremely vital information about the current state ";
	outFile << "of the program and it's known weaknesses/shortcomings.\n" << endl;
	outFile << "Major requirements to complete: " << endl;

    //Iterate through i n needed and output each unsatisfied major req
	for(map<string,string>::iterator itr = pairMH.first->begin(); itr != pairMH.first->end(); ++itr)
		outFile << itr->second << endl << itr->first << endl;

	//HASS
	outFile << endl << "HASS requirements to complete: "  << endl;

	for(map<string,string>::iterator itr = pairMH.second->begin(); itr != pairMH.second->end(); ++itr)
		outFile << itr->second << endl << itr->first << endl;

	outFile.close();
}

//int main is used for modular testing of the algorithm itself
//Will not be present in any deployments
//It calls the runAlgo function like the gui does and takes command line arguments and outputs to a file
int main(int argc, const char* args[]){
	/*if(args[1]==NULL || args[2]==NULL){
	  cerr << "One or more arguments is NULL." << endl;
	  return 0;
	  }

	  vector<string> comments;
	  vector<int> needed;
	  string input_file_name= args[1];
	  string classes_fname = args[2];
	  string OUTPUT_FILE_NAME = "Test_Files/algorithm_output.txt";
	  courseMap classes;
      Algo::read_classes(classes, classes_fname);
      pair<map<string, string>*, map<string, string>* > pairMH = Algo::runAlgo(input_file_name, classes);

      Algo::file_output(OUTPUT_FILE_NAME, pairMH);*/
}
#endif

}
