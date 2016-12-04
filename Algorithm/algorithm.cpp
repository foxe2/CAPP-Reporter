//Thomas Passaro

/*To do:
	- Dynamic memory for reqs vector and dictionary??
	- Implement credits counting
	- Pointers
	- Output
	- Compare
*/


#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>

using namespace std;

//Takes in a datastructure which it will add classes to
void parse_reqs(vector<vector<string> > &reqs, string &f_name){
	ifstream inFile(f_name);
	if (inFile.is_open()){
		string temp;
    	while(getline(inFile,temp)){
    		//If comment then skip
		    if(temp.find("//") != -1)
		    	continue;

		    vector<string> options;
		    //Loop through and parse
	    	int a = temp.find("||");
	    	while(a != -1){
	    		string course = temp.substr(0,a-1);
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
    }
    else
    	cout << "error";
	inFile.close();
}

bool special_compare(string &req, set<string> &classes, vector<string> &unacceptable, bool noRepeat){
	//Iterate through set checking for a match
	for(set<string>::iterator itr = classes.begin(); itr != classes.end(); ++itr){
		int courseNum = stoi(itr->substr(5));
		int reqNum = stoi(req.substr(5,4));
		bool isUnacceptable = false;
		for(int j = 0; j < unacceptable.size(); ++j)
			if(unacceptable[j].compare(*itr) == 0)
				isUnacceptable = true;
		if(courseNum >= reqNum  && !isUnacceptable){
			if(noRepeat)
				classes.erase(*itr);
			return true;
		}
	}
	return false;
}

bool concentration_compare(string &concentration, set<string> &classes){
	//Checks if repition in multiple requirements is allowed (ex Comm intensive)
	bool canRepeat;
	if(concentration.substr(0,1).compare("#") == 0){
		concentration = concentration.substr(1);
		canRepeat = true;
	}
	else
		canRepeat = false;

	int num_courses = stoi(concentration.substr(0,1));
	concentration = concentration.substr(2);
	int a = concentration.find("&&");
	string f_name;
	if(a == -1)
		f_name = "../Concentrations/" + concentration + ".txt";
	else
		f_name = "../Concentrations/" + concentration.substr(0,a) + ".txt";
	vector<vector<string> > conc_reqs;
	vector<int> not_taken;
	parse_reqs(conc_reqs, f_name);
	//  IMPLEMENT COURSE COUNTING
	compare_courses(conc_reqs, classes, not_taken, !canRepeat, num_courses);
	if(conc_reqs.size()-not_taken.size() == num_courses)
		return true;
}

void compare_courses(vector<vector<string> > &reqs, set<string> &classes, vector<int> &needed, bool noRepeat){
	for(int i = 0; i < reqs.size(); ++i){
		//Find unacceptable course
		set<string> unacceptable;
		int before = 0;
		while(reqs[i][before].substr(0,1).compare("!") == 0){
			unacceptable.insert(reqs[i][before].substr(1));
			before++;
		}

		bool satisfied = false;
		for(int j = before; j < reqs.size(); ++j){
			string temp = reqs[i][j];
			//Range(CSCI-4000+)
			if(temp.substr(0,1).compare("@") == 0){
				satisfied = concentration_compare(temp.substr(1), classes);
			}
			else if(temp.find("+") != -1){
				satisfied = special_compare(temp, classes, unacceptable, noRepeat);
			}
			//Regular courses
			else{
				if(classes.find(temp) != classes.end()){
					satisfied = true;
					if(noRepeat)
						classes.erase(reqs[i][reset_to]);
				}
			}
			if(satisfied)
				break;
		}
		if(!satisfied)
			needed.push_back(i);
	}
}

bool file_output(vector<vector<string> > &reqs, vector<int> &needed, string f_name){
	ofstream outFile(f_name);
  	if (!outFile.is_open())
    	return false;
    outFile << "CAUTION: Read the READ.ME before using this program." << endl;
    outFile << "It contains extremely vital information about the current state of the program and it's known weaknesses/shortcomings." << endl;
	//Loop through in needed and output each req
	for(int i = 0; i < needed.size(); ++i){
		for(int j = 0; j < reqs[needed[i]].size(); ++j){
			outFile << reqs[needed[i]][j] << " ";
		}
		outFile << endl;
	}
	outFile.close();
	return true;
}

pair<map<string, string>, map<string, string> > generateCappReport(string input_file, map<string, int> courses){
	vector<vector<string> > reqs;
	vector<int> needed;
	string input_file_name= args[1];
	const string OUTPUT_FILE_NAME = "algorithm_output.txt";

	parse_reqs(reqs, f_name);

	for(int i = 0; i < reqs.size(); ++i){
		for(int j = 0; j < reqs[i].size(); ++j)
			cout << reqs[i][j] << " ";
		cout << endl;
	}
	//parse_major_reqs(reqs);

	file_output(reqs, needed, OUTPUT_FILE_NAME);
}

