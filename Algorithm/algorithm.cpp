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
#include <string>
#include <map>

using namespace std;

void parse_reqs(string &f_name, vector<vector<string> > &major_reqs, vector<string> &major_comments, vector<vector<string> > &hass_reqs, vector<string> &hass_comments);
void parse_reqs(vector<vector<string> > &reqs, string &f_name);
bool special_compare(string &req, map<string, int> &classes, set<string> &unacceptable, bool noRepeat);
int free_electives(map<string, int> &classes, string credits);
int concentration_compare(string &concentration, map<string, int> &classes_credits);
void compare_courses(vector<vector<string> > &reqs, map<string, int> &classes, vector<int> &needed, bool noRepeat, int num_courses);
void compare_courses(vector<vector<string> > &reqs, map<string, int> &classes, vector<int> &needed);

//Takes in a datastructure which it will add classes to
void parse_reqs(string &f_name, vector<vector<string> > &major_reqs, vector<string> &major_comments, vector<vector<string> > &hass_reqs, vector<string> &hass_comments){
	ifstream inFile(f_name);
	if (inFile.is_open()){
		string temp;
		string most_recent_comment = "";
    	bool inHass = false;
    	while(getline(inFile,temp)){
    		if(temp.find("// HASS Requirement") != -1){
    			inHass = true;
    			continue;
    		}
    		//If comment then update field
    		int comment_index = temp.find("//");
		    if( comment_index != -1){
		    	most_recent_comment = temp.substr(comment_index+2);
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
	    		hass_reqs.push_back(options);
	    		hass_comments.push_back(most_recent_comment);
	    	}
	    	else{
	    		major_reqs.push_back(options);
	    		major_comments.push_back(most_recent_comment);
	    	}
    	}
    	inFile.close();
    }
    else
    	cout << "Could not open " << f_name << endl;
}

//Takes in a datastructure which it will add classes to
void parse_reqs(vector<vector<string> > &reqs, string &f_name){
	ifstream inFile(f_name);
	if (inFile.is_open()){
		string temp;
		string most_recent_comment = "";
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
    	cout << "Could not open " << f_name << endl;
}

bool special_compare(string &req, map<string, int> &classes, set<string> &unacceptable, bool noRepeat){
	//Iterate through set checking for a match
	for(map<string, int>::iterator itr = classes.begin(); itr != classes.end(); ++itr){
		int courseNum = stoi(itr->first.substr(5,4));
		int reqNum = stoi(req.substr(5,4));
		bool isUnacceptable = false;
		if(itr->first.substr(0,4).compare(req.substr(0,4)) == 0 && courseNum >= reqNum  && unacceptable.find(itr->first) == unacceptable.end()){
			if(noRepeat)
				classes.erase(itr);
			return true;
		}
	}
	return false;
}

int free_electives(map<string, int> &classes, string credits){
	int numCreds = stoi(credits);
	int currentCreds = 0;
	for(map<string, int>::iterator itr = classes.begin(); itr != classes.end(); ++itr){
		currentCreds += itr->second;
		if(currentCreds >= numCreds)
			return 0;
	}
	return numCreds - currentCreds;
}

int concentration_compare(string &concentration, map<string, int> &classes_credits){
	//Checks if repition in multiple requirements is allowed (ex Comm intensive)
	bool noRepeat = true;
	if(concentration.substr(0,1).compare("#") == 0){
		concentration = concentration.substr(1);
		noRepeat = false;
	}
	cout << concentration << endl;
	int num_courses = stoi(concentration.substr(0,1));
	cout << num_courses <<endl;
	string c_name = "Database/Concentrations/" + concentration.substr(2) + ".txt";
	vector<vector<string> > conc_reqs;
	vector<int> not_taken;
	parse_reqs(conc_reqs, c_name);
	// IMPLEMENT COURSE COUNTING
	compare_courses(conc_reqs, classes_credits, not_taken, noRepeat, num_courses);
	for(int i = 0; i < conc_reqs.size(); ++i){
		for(int j = 0; j < conc_reqs[i].size(); ++j)
			cout << conc_reqs[i][j] << " ";
		cout << endl;
	}
	cout<< "NEEDED: " << endl;
	for(int i = 0; i < not_taken.size(); ++i)
		cout<< not_taken[i];
	cout << endl<<"done"<<endl;
	if(conc_reqs.size()-not_taken.size() >= num_courses)
		return -1;
	cout << "res: " << conc_reqs.size()-not_taken.size()<<endl;
	return conc_reqs.size()-not_taken.size();
}

void compare_courses(vector<vector<string> > &reqs, map<string, int> &classes, vector<int> &needed, bool noRepeat, int num_courses){
	cout << "reqs size:  " << reqs.size()<<endl;
	vector<int> classes_to_delete;
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
		if(i-needed.size()+1 >= num_courses){
			cout<< needed.size()<< endl;
			cout << num_courses<<"i: "<<i<<endl;
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

void read_classes(map<string, int> &classes, string fname){
	ifstream inFile(fname);
	if (inFile.is_open()){
		string temp;
    	while(getline(inFile,temp)){
    		//If comment then skip
		    if(temp.find("//") != -1)
		    	continue;
		    int index = temp.find(":");
		    classes[temp.substr(0, index)] = stoi(temp.substr(index+1));
		}
		inFile.close();
    }
    else
    	cout << "error";
}

void file_output(string f_name, pair<map<string, string>*, map<string, string>* > pairMH){
	ofstream outFile(f_name);
  	if (!outFile.is_open())
    	cout << "Error writing to output file";
    outFile << "CAUTION: Read the READ.ME before using this program." << endl;
    outFile << "It contains extremely vital information about the current state of the program and it's known weaknesses/shortcomings." << endl << endl;
	//Loop through in needed and output each req
	outFile << "Major requirements to complete: " << endl;
	for(map<string,string>::iterator itr = pairMH.first->begin(); itr != pairMH.first->end(); ++itr)
		outFile << itr->second << endl << itr->first << endl;
	outFile << endl << "HASS requirements to complete: "  << endl;
	for(map<string,string>::iterator itr = pairMH.second->begin(); itr != pairMH.second->end(); ++itr)
		outFile << itr->second << endl << itr->first << endl;
	outFile.close();
}

pair<map<string, string>*, map<string, string>* > runAlgo(string req_file, map<string, int> courses){
	map<string, string>* hass = new map<string, string>();
	vector<vector<string> > hass_reqs;
	vector<int> hass_needed;
	vector<string> hass_comments;
	map<string, string>* major = new map<string, string>();
	vector<vector<string> > major_reqs;
	vector<int> major_needed;
	vector<string> major_comments;
	parse_reqs(req_file, major_reqs, major_comments, hass_reqs, hass_comments);
	compare_courses(hass_reqs, courses, hass_needed);
	for(int i = 0; i < hass_needed.size(); ++i){
		string temp = "";
		vector<string> components = hass_reqs[hass_needed[i]];
		for(int j = 0; j < components.size(); ++j)
			temp = temp + components[j] + " ";
		(*hass)[temp.substr(0,temp.length()-1)]= hass_comments[hass_needed[i]];
	}
	compare_courses(major_reqs, courses, major_needed);
	for(int i = 0; i < major_needed.size(); ++i){
		string temp = "";
		vector<string> components = major_reqs[major_needed[i]];
		for(int j = 0; j < components.size(); ++j)
			temp = temp + components[j] + " ";
		(*major)[temp.substr(0,temp.length()-1)] = major_comments[major_needed[i]];
	}
	return make_pair(major,hass);
}

int main(int argc, const char* args[]){
	if(args[1]==NULL || args[2]==NULL){
		cerr << "One or more arguments is NULL." << endl;
		return 0;
	}

	/*vector<vector<string> > reqs;
	vector<string> comments;
	vector<int> needed;*/
	string input_file_name= args[1];
	string classes_fname = args[2];
	string OUTPUT_FILE_NAME = "Test_Files/algorithm_output.txt";
	map<string, int> classes;
	read_classes(classes, classes_fname);
	pair<map<string, string>*, map<string, string>* > pairMH = runAlgo(input_file_name, classes);
	//parse_reqs(reqs, input_file_name, comments);
	
	//compare_courses(reqs, classes, needed);
	/*for(int i = 0; i < reqs.size(); ++i){
		for(int j = 0; j < reqs[i].size(); ++j)
			cout << reqs[i][j] << " ";
		cout << endl;
	}
	cout << endl;
	for(int i = 0; i <needed.size(); ++i)
		cout << needed[i] << endl;*/
	file_output(OUTPUT_FILE_NAME, pairMH);
}

