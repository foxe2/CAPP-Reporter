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
	
	//Open the file
	ifstream inFile(fName);

	//If the file opened successfully
	if (inFile.is_open()){

		//Initalize variables
		string line = "";

		//For each line in the file
		while(getline(inFile,line)){
            
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

//Calculates the amunt of free elective credits taken and how many credits are left to satisfy the requirements
int freeElectives(courseMap& classes, const string& credits){

	//Local variables
	int currentCreds = 0, numCreds = stoi(credits);

	if(numCreds==0) return 0;
	
	//Iterate through the courses left and add each credit value to currentCreds
	for(courseMap::iterator itr = classes.begin(); itr != classes.end(); ++itr){

		currentCreds += itr->second;

		//If they have satisfied the credit requirement then break
		if(currentCreds >= numCreds) return 0;
	}

	//Return difference
	return numCreds - currentCreds;
}

//Beginning function for comparing evaluating concentration requirements
bool concentrationCompare(const string& initConcentration, courseMap& classes) {

	//Local variables
	string concentration, cFileName;
	bool noRepeat = true;
	int numCourses;
	concentration = initConcentration;

	//A requirement data structure of all the requirements for a certain concentration file
	vector<string> concReqs;

	//Checks if repition in multiple requirements is allowed (ex Comm intensive)    
	if(concentration.substr(0,1).compare("#") == 0){
		concentration = concentration.substr(1);
		noRepeat = false;
	}

	//The number of classes to be taken in a concentrationCompare
	numCourses = stoi(concentration.substr(0,1));
	cFileName = "Database/Concentrations/" + concentration.substr(2) + ".txt";

	//Parse the concentration file for requirements
	parseConcentrationReqs(concReqs, cFileName);

	//Compare the classes to the concetration requirements
	return compareConcCourses(concReqs, classes, noRepeat, numCourses);
}

//Compares courses for the concentration requirements and
//determines which ones are satisfied
bool compareConcCourses(vector<string>& reqs, courseMap& classes, bool noRepeat, int numCourses) {

	//Determines the first index without an exclusion(ex. !CSCI-4020)
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

		//Parses for a series of && statements
		int index;
		while(true){
			
			//Finds next occurence of &&
			index = element.find("&&");

			//If && not found, assign temp as the element 
			if(index == -1) temp = element;

			//Else, use the next subelement and remove it from elemenet
			else{
				temp = element.substr(0,index);
				element = element.substr(index+2);
			}

			//If it's a concentration then call concentrationCompare()
			//This would start the concentration cycle over again(it's recursive, if necessary)
			if(temp.substr(0,1).compare("@") == 0)
				satisfied = concentrationCompare(temp.substr(1), classes);

			//If it's a range of classes (ex. CSCI-4000+)
			else if(temp.find("+") != -1)
                satisfied = specialCompare(temp, classes, unacceptable, noRepeat);

			//Regular courses
			else{
				//Checks that the class hasnt been in 'removed' yet
				//And that it is in the course set
				courseMap::iterator itr = classes.find(temp);
				if( itr != classes.end() && taken.find(itr->first) == taken.end()){
					satisfied = true;
					taken.insert(itr->first);
				}
			}

			//If a subelement of && statements is false 
			//or it is the end of the line then break
			if(!satisfied || index == -1) break;
		}

		//Reset the parsing location to allow for multiple reqs in the same range
		//Also tracks numCourses
		if(satisfied){
			--numCourses;
			--i;
		}

		//If it numCourses == 0, then enough reqs have been satisfied,
		//Erase the classes used                                       
		if (numCourses == 0){
			for(set<string>::iterator itr = taken.begin(); itr != taken.end(); ++itr)
				classes.erase(*itr);
			break;
		}
	}
	//Return true or false
	return numCourses <= 0;
}

//Compares requirements to taken classes
void compareCourses(reqsVector& reqs, courseMap& classes, vector<int>& needed) {

	//For each element in reqs (big requirements)
    for(int i = 0; i < (int)reqs.size(); ++i){

    	//Local variables
		set<string> unacceptable;
		bool satisfied = false;

		//Before marks where the algorithm should start checking for matches
		int before = 0;

        //Find unacceptable courses which will not count for this particular req
		while(reqs[i][before].substr(0,1).compare("!") == 0){
			unacceptable.insert(reqs[i][before].substr(1));
			before++;
		}

		//Loop through the rest of reqs[i] to work with the OR statements
        for(int j = before; j < (int)reqs[i].size(); ++j){

        	//Local variables
			int index; 
			string temp, element = reqs[i][j];

			//If element is a free elective requirement than handle it
			if(element.find("$") == 0){

				//Number of credits in free electives required to graduate
				string credits = element.substr(1);

				//Determine the number of missing credits
				int dif = freeElectives(classes, credits);

				//If dif is negative 
				if(dif > 0){
					vector<string> creds;
					//More user friendly version than requirement notation ($num)
					creds.push_back("You need " + to_string(dif) + " more free elective credits.");
					reqs[i] = creds;
				}

				else satisfied = true;
			}

			//If credits is satisfied dont continue
			if(satisfied) break;

			//If there are encapsulating parenthesis get rid of them
            else if((int)element.find("(") != -1)
				element = element.substr(1).substr(0,element.length()-2);

			//While parsing && statements continues
            while(true){

				satisfied = false;
				index = element.find("&&");

				//If last subelement then assign it to temp
                if(index == -1) temp = element;

                //Otherwise isolate and remove next subelement
				else{
					temp = element.substr(0,index);
					element = element.substr(index+2);
				}

				//If it is a concentration requirement then handle it
				if(temp.substr(0,1).compare("@") == 0)
                    satisfied = concentrationCompare(temp.substr(1), classes);

                //If it is a range (ex CSCI-4000+)
				else if(temp.find("+") != -1)
                    satisfied = specialCompare(temp, classes, unacceptable, true);

				//Regular(required) courses
				//If the class is in classes
				else if(classes.find(temp) != classes.end()){
					//Remove the required course from classes
					satisfied = true;
					classes.erase(reqs[i][j]);
				}

				//If a subelement of && statements is false
				//Or it is the end of the line, break
                if(!satisfied || index == -1) break;
			}

			//If the requirement was satisfied skip the rest of the loop
            if(satisfied) break;
		}

		//If the req isn't satisfied push_back the needed vector
		//Contains the index of the req in reqs
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

    //Iterates through unsatisfied requirements and adds it to return vector
    for(int i = 0; i < (int)hassNeeded.size(); ++i){

    	//temp is the string equivalent of the components vector
		string temp = "";
		//Equivalent to hassReqs[i], OR components of 1 requirement
		vector<string> components = hassReqs[hassNeeded[i]];

		//Appends each component to the string
        for(int j = 0; j < (int)components.size(); ++j) temp = temp + components[j] + " ";

        //Adds the string to the dictionary as a key with its comment as the value
		(*hass)[temp.substr(0,temp.length()-1)]= hassComments[hassNeeded[i]];

	}

    //Compares the necessary requirements for their major with the user's
	//completed classes and fills the missing requirements in majorNeeded
	compareCourses(majorReqs, courses, majorNeeded);

    //Iterates through unsatisfied requirements and adds it to return vector
    for(int i = 0; i < (int)majorNeeded.size(); ++i){

		//temp is the string equivalent of the components vector
		string temp = "";
		//Equivalent to hassReqs[i], OR components of 1 requirement
		vector<string> components = majorReqs[majorNeeded[i]];

		//Appends each component to the string
        for(int j = 0; j < (int)components.size(); ++j) temp = temp + components[j] + " ";

        //Adds the string to the dictionary as a key with its comment as the value
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
