#include "Algorithm.hpp"


//For ease of reading
using namespace std;

//Takes in a data structure which it will add
//classes to. This function returns false should it fail
bool parse_reqs(const string &fName, reqsVector& majorReqs,
		commentVector& majorComments, reqsVector& hassReqs, 
		commentVector& hassComments){

	//Open the file
    ifstream inFile(fName);

	//If there was no error
	if (inFile.is_open()){

		//Initalize variables                                                        //TODO: TEMP SEEMS IMPORTANT HERE! CHANGE THE NAME !!!    
		bool inHass = false;
		string temp, lastComment = "";

		//For each line in the requirements file
		while(getline(inFile,temp)){

			//If we finished parsing the main requirements, switch sections
            if((int)temp.find("// HASS Requirement") != -1){
				inHass = true;
				continue;
			}

			//If the line is a comment then update
			int tmp = temp.find("//");
            if(tmp != -1){
				lastComment = temp.substr(tmp+2);
				continue;
			}

            //																TODO:  What does this one do!!!? Comment it!
			vector<string> options;

			//Loop through and parse
			for(int a = temp.find("||"); a != -1; a = temp.find("||")) {

				//Takes the next course 
				string course = temp.substr(0,a);

				//If it cant be taken to satisfy req
				if(course.substr(0,1).compare("!") == 0)
					options.insert(options.begin(), course);
				else options.push_back(course);

				temp=temp.substr(a+2);
			}
			if(temp.substr(0,1).compare("!") == 0)
				options.insert(options.begin(),temp);
			else
				options.push_back(temp);
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
void parse_reqs(reqsVector& reqs, const string &fName){

	//Open the file
	ifstream inFile(fName);

	//If the file opened successfully
	if (inFile.is_open()){

		//Initalize variables                                                    <TODO:  TEMP SEEMS IMPORTANT HERE. RENAME IT !
		string temp, mostRecentComment = "";

		//For each line in the file
		while(getline(inFile,temp)){

			//If it is a comment then update field                                                <TODO:  THIS COMMENT SEEMS FALSE !
            if( (int)temp.find("//") != -1) continue;

            //																TODO:  What does this one do!!!? Comment it!
            vector<string> options;

			//Loop through and parse
			for(int a = temp.find("||"); a != -1; a = temp.find("||")) {

				//Read the course
				string course = temp.substr(0,a);

				//If it cant be taken to satisfy req
				if(course.substr(0,1).compare("!") == 0)
					options.insert(options.begin(), course);

				else options.push_back(course);

				temp=temp.substr(a+2);
			}

			if(temp.substr(0,1).compare("!") == 0)
				options.insert(options.begin(),temp);

			else options.push_back(temp);

			reqs.push_back(options);
		}

		//Close the file
		inFile.close();
	}

	//If the file failed to open, note so
	else cout << "Could not open " << fName << endl;
}

//Takes a class range such as CSCI-4000+ and checks to see if any class satisfies that requirement
//Accounts for any classes that do not count in unacceptable
bool specialCompare(const string &req, courseMap& classes, const set<string>& unacceptable, bool noRepeat){

    //Iterate through set checking for a match
	for(map<string, int>::iterator itr = classes.begin(); 
			itr != classes.end(); ++itr){

 cout << "MADE IT TO LINE:  " << __LINE__ << itr->first.c_str();
        int courseNum = stoi(itr->first.substr(5,4));
		int reqNum = stoi(req.substr(5,4));

 cout << "MADE IT TO LINE:  " << __LINE__ << endl;
        //If the course subjects match (ex CSCI) and the course number
		//is greater than the required level and it is not unacceptable
		if(itr->first.substr(0,4).compare(req.substr(0,4)) == 0 && 
				unacceptable.find(itr->first) == unacceptable.end() &&
				courseNum >= reqNum) {

 cout << "MADE IT TO LINE:  " << __LINE__ << endl;
            //Erases the class if it cannot be double counted
			if(noRepeat) classes.erase(itr);

			return true;
		}
	}

	return false;
}

int free_electives(map<string, int> &classes, const string &credits){

	int currentCreds = 0, numCreds = stoi(credits);

	for(map<string, int>::iterator itr = classes.begin();
			itr != classes.end(); ++itr){

		currentCreds += itr->second;

		if(currentCreds >= numCreds) return 0;
	}

	return numCreds - currentCreds;
}

int concentration_compare(const string &initConcentration, map<string, int> &classes_credits) {

	//Local variables
	string concentration, cName;
	bool noRepeat = true;
	int numCourses;

	//                   <-----------------------------------------------------------------EXPLAIN WHAT THESE ARE
	vector<vector<string> > conc_reqs;
	vector<int> notTaken;

	//Checks if repition in multiple requirements is allowed (ex Comm intensive)    
	concentration = initConcentration;
	if(concentration.substr(0,1).compare("#") == 0){
		concentration = concentration.substr(1);
		noRepeat = false;
	}

	cout << concentration << endl;                                        //THIS

	numCourses = stoi(concentration.substr(0,1));

	cout << numCourses <<endl;                                           //SHOULD BE HERE AS cout << concentration << '\n' << numCourses << endl;

	cName = "Database/Concentrations/" + concentration.substr(2) + ".txt";

	parse_reqs(conc_reqs, cName);

	// IMPLEMENT COURSE COUNTING                                                            CHANGE COMMENT
	compare_courses(conc_reqs, classes_credits, notTaken, noRepeat, numCourses);

    for(int i = 0; i < (int)conc_reqs.size(); ++i){
        for(int j = 0; j < (int)conc_reqs[i].size(); ++j)
			cout << conc_reqs[i][j] << " ";
		cout << endl;
	}

	cout<< "NEEDED: " << endl;

    for(int i = 0; i < (int)notTaken.size(); ++i) cout<< notTaken[i];

	cout << endl<<"done"<<endl;

    if((int)(conc_reqs.size()-notTaken.size()) >= numCourses) return -1;

	cout << "res: " << conc_reqs.size()-notTaken.size()<<endl;

	return conc_reqs.size()-notTaken.size();
}

void compare_courses(reqsVector& reqs, courseMap &classes,
		vector<int> &needed, bool noRepeat, int numCourses) {

	cout << "reqs size:  " << reqs.size()<<endl;

	//For each requierment
    for(int i = 0; i < (int)reqs.size(); ++i){

		int before = 0;
		bool satisfied = false;
		set<string> unacceptable;

		//Find unacceptable course        
		while(reqs[i][before].substr(0,1).compare("!") == 0){
			unacceptable.insert(reqs[i][before].substr(1));
			before++;
		}


        for(int j = before; j < (int)reqs[i].size(); ++j){

			int index; 
			string temp;
			string element = reqs[i][j];

			if(element.find("$") == 0){

				string credits = element.substr(1);
				int dif = free_electives(classes, credits);

				if(dif > 0){
					vector<string> creds;
					creds.push_back("You need " + to_string(dif) + " more free elective credits.");
					reqs[i] = creds;
				}

				else satisfied = true;
			}

            else if((int)element.find("(") != -1)
				element = element.substr(1,element.length()-2);

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
					if(concentration_compare(temp.substr(1), classes) == -1) 
						satisfied = true;

				}

				else if(temp.find("+") != -1)
                    satisfied = specialCompare(temp, classes, unacceptable, noRepeat);

				//Regular courses
				else if(classes.find(temp) != classes.end()){

					satisfied = true;
					if(noRepeat) classes.erase(reqs[i][j]);
				}

				if(!satisfied || index == -1) break;
			}

			if(satisfied) break;
		}

		if(!satisfied) needed.push_back(i);

        if(i-(int)needed.size()+1 >= numCourses){
			cout<< needed.size()<< endl;
			cout << numCourses<<"i: "<<i<<endl;
			break;
		}
	}
}


void compare_courses(reqsVector &reqs, courseMap &classes, vector<int> &needed) {

    for(int i = 0; i < (int)reqs.size(); ++i){

		set<string> unacceptable;
		bool satisfied = false;
		int before = 0;

        //Find unacceptable course
		while(reqs[i][before].substr(0,1).compare("!") == 0){
			unacceptable.insert(reqs[i][before].substr(1));
			before++;
		}

        for(int j = before; j < (int)reqs[i].size(); ++j){

			int index; 
			string temp, element = reqs[i][j];

			if(element.find("$") == 0){

				string credits = element.substr(1);
				int dif = free_electives(classes, credits);

				if(dif > 0){
					vector<string> creds;
					creds.push_back("You need " + to_string(dif) + " more free elective credits.");
					reqs[i] = creds;
				}

				else satisfied = true;
			}

            else if((int)element.find("(") != -1)
				element = element.substr(1).substr(0,element.length()-2);

 cout << "MADE IT TO LINE:  " << __LINE__ << endl;
            while(true){

				satisfied = false;
				index = element.find("&&");

 cout << "MADE IT TO LINE:  " << __LINE__ << endl;
                if(index == -1) temp = element;

				else{
 cout << "MADE IT TO LINE:  " << __LINE__ << endl;

					temp = element.substr(0,index);
					element = element.substr(index+2);
				}

				//Range(CSCI-4000+)
				if(temp.substr(0,1).compare("@") == 0) {

 cout << "MADE IT TO LINE:  " << __LINE__ << endl;
                    if(concentration_compare(temp.substr(1), classes) == -1)
						satisfied = true;
				}

				else if(temp.find("+") != -1)
                    satisfied = specialCompare(temp, classes, unacceptable, true);

				//Regular courses
				else if(classes.find(temp) != classes.end()){
 cout << "MADE IT TO LINE:  " << __LINE__ << endl;

					satisfied = true;
					classes.erase(reqs[i][j]);
				}
 cout << "MADE IT TO LINE:  " << __LINE__ << endl;
                if(!satisfied || index == -1) break;


			}

 cout << "MADE IT TO LINE:  " << __LINE__ << endl;
            if(satisfied) break;
		}

 cout << "MADE IT TO LINE:  " << __LINE__ << endl;
        if(!satisfied) needed.push_back(i);
	}
}

//Read classes is only used in main() for modular testing
//Reads in a text file with one course per line
//Users enter classes in the GUI and this is not present in a deployment
void read_classes(map<string, int> &classes, string fname){

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
void file_output(string fName, pair<outputMap*, outputMap* > pairMH){

	ofstream outFile(fName);

	if (!outFile.is_open()) cout << "Error writing to output file";

	outFile << "CAUTION: Read the READ.ME before using this program." << endl;
	outFile << "It contains extremely vital information about the current state ";
	outFile << "of the program and it's known weaknesses/shortcomings.\n" << endl;
	outFile << "Major requirements to complete: " << endl;

	//Iterate through in needed and output each unsatisfied major req
	for(map<string,string>::iterator itr = pairMH.first->begin(); itr != pairMH.first->end(); ++itr)
		outFile << itr->second << endl << itr->first << endl;

	//HASS
	outFile << endl << "HASS requirements to complete: "  << endl;

	for(map<string,string>::iterator itr = pairMH.second->begin(); itr != pairMH.second->end(); ++itr)
		outFile << itr->second << endl << itr->first << endl;

	outFile.close();
}

//This is called by the GUI.
//Returns a pair of maps where each map contains key-value pairs of an
//unsatisfied requirement and a comment about the requirement. The first element 
//in the pair is a map with Major requirements in it, while the second is for HASS
pair<outputMap*, outputMap*> runAlgo(const string &req_file, courseMap courses){

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
    if (!parse_reqs(req_file, majorReqs, majorComments, hassReqs, hassComments))
        return make_pair(major,hass);
 cout << "MADE IT TO LINE:  " << __LINE__ << endl;

	//Compares the necessary requirements for hass with the user's
	//completed classes and fills the missing requirements in hassNeeded
	compare_courses(hassReqs, courses, hassNeeded);

 cout << "MADE IT TO LINE: " << __LINE__ << endl;
    //Assigns each missing req with its comment in the hass map
    for(int i = 0; i < (int)hassNeeded.size(); ++i){

		string temp = "";
		vector<string> components = hassReqs[hassNeeded[i]];

        for(int j = 0; j < (int)components.size(); ++j) temp = temp + components[j] + " ";

		(*hass)[temp.substr(0,temp.length()-1)]= hassComments[hassNeeded[i]];

	}

 cout << "MADE IT TO LINE: " << __LINE__ << endl;
    //Compares the requirements for major
	compare_courses(majorReqs, courses, majorNeeded);

 cout << "MADE IT TO LINE: " << __LINE__ << endl;
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
	  map<string, int> classes;
	  read_classes(classes, classes_fname);
	  pair<map<string, string>*, map<string, string>* > pairMH = runAlgo(input_file_name, classes);

	  file_output(OUTPUT_FILE_NAME, pairMH);*/
}
#endif
