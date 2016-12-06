//Thomas Passaro

/*To do:
	- Documentation
	- User Friendly output
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <map>

using namespace std;

void parse_reqs(const string &fName, vector<vector<string> > &majorReqs, vector<string> &majorComments, vector<vector<string> > &hassReqs, vector<string> &hassComments);
void parse_reqs(vector<vector<string> > &reqs, const string &fName);
bool special_compare(const string &req, map<string, int> &classes, const set<string> &unacceptable, bool noRepeat);
int free_electives(map<string, int> &classes, const string &credits);
int concentration_compare(const string &initConcentration, map<string, int> &classes_credits);
void compare_courses(vector<vector<string> > &reqs, map<string, int> &classes, vector<int> &needed, bool noRepeat, int numCourses);
void compare_courses(vector<vector<string> > &reqs, map<string, int> &classes, vector<int> &needed);
pair<map<string, string>*, map<string, string>* > runAlgo(const string &req_file, map<string, int> courses);

//Takes in a datastructure which it will add classes to
void parse_reqs(const string &fName, vector<vector<string> > &majorReqs, vector<string> &majorComments, vector<vector<string> > &hassReqs, vector<string> &hassComments){
	ifstream inFile(fName);
	if (inFile.is_open()){
		string temp;
		string mostRecentComment = "";
    	bool inHass = false;
    	while(getline(inFile,temp)){
    		if(temp.find("// HASS Requirement") != -1){
    			inHass = true;
    			continue;
    		}
    		//If comment then update field
    		int commentIndex = temp.find("//");
		    if( commentIndex != -1){
		    	mostRecentComment = temp.substr(commentIndex+2);
		    	continue;
		    }

		    vector<string> options;
		    //Loop through and parse
	    	int a = temp.find("||");
	    	while(a != -1){
	    		string course = temp.substr(0,a);
	    		//If it cant be taken to satisfy req
	    		if(course.substr(0,1).compare("!") == 0)
	    			options.insert(options.begin(), course);
	    		else
	    			options.push_back(course);
	    		temp=temp.substr(a+2);
	    		a = temp.find("||");
	    	}
	    	if(temp.substr(0,1).compare("!") == 0)
	    		options.insert(options.begin(),temp);
	    	else
	    		options.push_back(temp);
	    	if(inHass && options[0].find("$") == -1){
	    		hassReqs.push_back(options);
	    		hassComments.push_back(mostRecentComment);
	    	}
	    	else{
	    		majorReqs.push_back(options);
	    		majorComments.push_back(mostRecentComment);
	    	}
    	}
    	inFile.close();
    }
    else
    	cout << "Could not open " << fName << endl;
}

//Takes in a datastructure which it will add classes to
void parse_reqs(vector<vector<string> > &reqs, const string &fName){
	ifstream inFile(fName);
	if (inFile.is_open()){
		string temp;
		string mostRecentComment = "";
    	while(getline(inFile,temp)){
    		//If comment then update field
		    if( temp.find("//") != -1)
		    	continue;

		    vector<string> options;
		    //Loop through and parse
	    	int a = temp.find("||");
	    	while(a != -1){
	    		string course = temp.substr(0,a);
	    		//If it cant be taken to satisfy req
	    		if(course.substr(0,1).compare("!") == 0)
	    			options.insert(options.begin(), course);
	    		else
	    			options.push_back(course);
	    		temp=temp.substr(a+2);
	    		a = temp.find("||");
	    	}
	    	if(temp.substr(0,1).compare("!") == 0)
	    		options.insert(options.begin(),temp);
	    	else
	    		options.push_back(temp);
	    	reqs.push_back(options);
    	}
    	inFile.close();
    }
    else
    	cout << "Could not open " << fName << endl;
}

//Takes a class range such as CSCI-4000+ and checks to see if any class satisfies that requirement
//Accounts for any classes that do not count in unacceptable
bool special_compare(const string &req, map<string, int> &classes, const set<string> &unacceptable, bool noRepeat){
	//Iterate through set checking for a match
	for(map<string, int>::iterator itr = classes.begin(); itr != classes.end(); ++itr){
		int courseNum = stoi(itr->first.substr(5,4));
		int reqNum = stoi(req.substr(5,4));
		bool isUnacceptable = false;
		//If the course subjects match (ex CSCI) and the course number is greater than the required level and it is not unacceptable
		if(itr->first.substr(0,4).compare(req.substr(0,4)) == 0 && courseNum >= reqNum  && unacceptable.find(itr->first) == unacceptable.end()){
			//Erases the class if it cannot be double counted
			if(noRepeat)
				classes.erase(itr);
			return true;
		}
	}
	return false;
}

int free_electives(map<string, int> &classes, const string &credits){
	int numCreds = stoi(credits);
	int currentCreds = 0;
	for(map<string, int>::iterator itr = classes.begin(); itr != classes.end(); ++itr){
		currentCreds += itr->second;
		if(currentCreds >= numCreds)
			return 0;
	}
	return numCreds - currentCreds;
}

int concentration_compare(const string &initConcentration, map<string, int> &classes_credits){
	//Checks if repition in multiple requirements is allowed (ex Comm intensive)
	string concentration = initConcentration;
	bool noRepeat = true;
	if(concentration.substr(0,1).compare("#") == 0){
		concentration = concentration.substr(1);
		noRepeat = false;
	}
	cout << concentration << endl;
	int numCourses = stoi(concentration.substr(0,1));
	cout << numCourses <<endl;
	string cName = "Database/Concentrations/" + concentration.substr(2) + ".txt";
	vector<vector<string> > conc_reqs;
	vector<int> notTaken;
	parse_reqs(conc_reqs, cName);
	// IMPLEMENT COURSE COUNTING
	compare_courses(conc_reqs, classes_credits, notTaken, noRepeat, numCourses);
	for(int i = 0; i < conc_reqs.size(); ++i){
		for(int j = 0; j < conc_reqs[i].size(); ++j)
			cout << conc_reqs[i][j] << " ";
		cout << endl;
	}
	cout<< "NEEDED: " << endl;
	for(int i = 0; i < notTaken.size(); ++i)
		cout<< notTaken[i];
	cout << endl<<"done"<<endl;
	if(conc_reqs.size()-notTaken.size() >= numCourses)
		return -1;
	cout << "res: " << conc_reqs.size()-notTaken.size()<<endl;
	return conc_reqs.size()-notTaken.size();
}

void compare_courses(vector<vector<string> > &reqs, map<string, int> &classes, vector<int> &needed, bool noRepeat, int numCourses){
	cout << "reqs size:  " << reqs.size()<<endl;
	for(int i = 0; i < reqs.size(); ++i){
		//Find unacceptable course
		set<string> unacceptable;
		int before = 0;
		while(reqs[i][before].substr(0,1).compare("!") == 0){
			unacceptable.insert(reqs[i][before].substr(1));
			before++;
		}
		bool satisfied = false;
		for(int j = before; j < reqs[i].size(); ++j){
			string element = reqs[i][j];
			if(element.find("$") == 0){
				string credits = element.substr(1);
				int dif = free_electives(classes, credits);
				if(dif > 0){
					vector<string> creds;
					creds.push_back("You need " + to_string(dif) + " more free elective credits.");
					reqs[i] = creds;
				}
				else
					satisfied = true;
			}
			else if(element.find("(") != -1)
				element = element.substr(1).substr(0,element.length()-2);
				int index; 
				string temp;
			while(true){
				satisfied = false;
				index = element.find("&&");
				if(index == -1)
					temp = element;
				else{
					temp = element.substr(0,index);
					element = element.substr(index+2);
				}
				//Range(CSCI-4000+)
				if(temp.substr(0,1).compare("@") == 0){
					string name = temp.substr(1);
					int num_left = concentration_compare(name, classes);
					if(num_left == -1)
						satisfied = true;
				}
				else if(temp.find("+") != -1){
					satisfied = special_compare(temp, classes, unacceptable, noRepeat);
				}
				//Regular courses
				else{
					if(classes.find(temp) != classes.end()){
						satisfied = true;
						if(noRepeat)
							classes.erase(reqs[i][j]);
					}
				}
				if(!satisfied)
					break;
				if(index == -1)
					break;
			}
			if(satisfied)
				break;
		}
		if(!satisfied)
			needed.push_back(i);
		if(i-needed.size()+1 >= numCourses){
			cout<< needed.size()<< endl;
			cout << numCourses<<"i: "<<i<<endl;
			break;
		}
	}
}

void compare_courses(vector<vector<string> > &reqs, map<string, int> &classes, vector<int> &needed){
	for(int i = 0; i < reqs.size(); ++i){
		//Find unacceptable course
		set<string> unacceptable;
		int before = 0;
		while(reqs[i][before].substr(0,1).compare("!") == 0){
			unacceptable.insert(reqs[i][before].substr(1));
			before++;
		}
		bool satisfied = false;
		for(int j = before; j < reqs[i].size(); ++j){
			string element = reqs[i][j];
			if(element.find("$") == 0){
				string credits = element.substr(1);
				int dif = free_electives(classes, credits);
				if(dif > 0){
					vector<string> creds;
					creds.push_back("You need " + to_string(dif) + " more free elective credits.");
					reqs[i] = creds;
				}
				else
					satisfied = true;
			}
			else if(element.find("(") != -1)
				element = element.substr(1).substr(0,element.length()-2);
				int index; 
				string temp;
			while(true){
				satisfied = false;
				index = element.find("&&");
				if(index == -1)
					temp = element;
				else{
					temp = element.substr(0,index);
					element = element.substr(index+2);
				}
				//Range(CSCI-4000+)
				if(temp.substr(0,1).compare("@") == 0){
					string name = temp.substr(1);
					int num_left = concentration_compare(name, classes);
					if(num_left == -1)
						satisfied = true;
				}
				else if(temp.find("+") != -1){
					satisfied = special_compare(temp, classes, unacceptable, true);
				}
				//Regular courses
				else{
					if(classes.find(temp) != classes.end()){
						satisfied = true;
						classes.erase(reqs[i][j]);
					}
				}
				if(!satisfied)
					break;
				if(index == -1)
					break;
			}
			if(satisfied)
				break;
		}
		if(!satisfied)
			needed.push_back(i);
	}
}

//Read classes is only used in main() for modular testing
//Reads in a text file with one course per line
//Users enter classes in the GUI and this is not present in a deployment
void read_classes(map<string, int> &classes, string fname){
	ifstream inFile(fname);
	if (inFile.is_open()){
		string temp;
    	while(getline(inFile,temp)){
    		//If comment then skip
		    if(temp.find("//") != -1)
		    	continue;
		    int index = temp.find(":");
		    //Adds each class with the course name as the key and number of credits as the value
		    classes[temp.substr(0, index)] = stoi(temp.substr(index+1));
		}
		inFile.close();
    }
    else
    	cout << "error";
}

//Only used for modular testing and is called solely in main()
//Outputs the data to a file 
void file_output(string fName, pair<map<string, string>*, map<string, string>* > pairMH){
	ofstream outFile(fName);
  	if (!outFile.is_open())
    	cout << "Error writing to output file";
    outFile << "CAUTION: Read the READ.ME before using this program." << endl;
    outFile << "It contains extremely vital information about the current state of the program and it's known weaknesses/shortcomings." << endl << endl;
	//Iterate through in needed and output each unsatisfied major req
	outFile << "Major requirements to complete: " << endl;
	for(map<string,string>::iterator itr = pairMH.first->begin(); itr != pairMH.first->end(); ++itr)
		outFile << itr->second << endl << itr->first << endl;
	//HASS
	outFile << endl << "HASS requirements to complete: "  << endl;
	for(map<string,string>::iterator itr = pairMH.second->begin(); itr != pairMH.second->end(); ++itr)
		outFile << itr->second << endl << itr->first << endl;
	outFile.close();
}

//This is called by the GUI.
//Returns a pair of maps where each map contains key-value pairs of an unsatisfied requirement and a comment about the requirement
//The first element in the pair is a map with Major requirements in it, while the second is for HASS
pair<map<string, string>*, map<string, string>* > runAlgo(const string &req_file, map<string, int> courses){
	//Initializes all hass data structures
	map<string, string>* hass = new map<string, string>();
	vector<vector<string> > hassReqs;
	vector<int> hassNeeded;
	vector<string> hassComments;
	//Initializes all major data structures
	map<string, string>* major = new map<string, string>();
	vector<vector<string> > majorReqs;
	vector<int> majorNeeded;
	vector<string> majorComments;
	//Parses the requirements file and puts the requirements and comments in their corresponding data structures
	parse_reqs(req_file, majorReqs, majorComments, hassReqs, hassComments);
	//Compares the necessary requirements for hass with the user's completed classes and fills the missing requirements in hassNeeded
	compare_courses(hassReqs, courses, hassNeeded);
	//Assigns each missing req with its comment in the hass map
	for(int i = 0; i < hassNeeded.size(); ++i){
		string temp = "";
		vector<string> components = hassReqs[hassNeeded[i]];
		for(int j = 0; j < components.size(); ++j)
			temp = temp + components[j] + " ";
		(*hass)[temp.substr(0,temp.length()-1)]= hassComments[hassNeeded[i]];
	}
	//Compares the requirements for major 
	compare_courses(majorReqs, courses, majorNeeded);
	//Generates major map
	for(int i = 0; i < majorNeeded.size(); ++i){
		string temp = "";
		vector<string> components = majorReqs[majorNeeded[i]];
		for(int j = 0; j < components.size(); ++j)
			temp = temp + components[j] + " ";
		(*major)[temp.substr(0,temp.length()-1)] = majorComments[majorNeeded[i]];
	}
	//Returns the pair of maps.
	return make_pair(major,hass);
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
	map<string, int> classes;
	read_classes(classes, classes_fname);
	pair<map<string, string>*, map<string, string>* > pairMH = runAlgo(input_file_name, classes);
	
	file_output(OUTPUT_FILE_NAME, pairMH);*/
}

