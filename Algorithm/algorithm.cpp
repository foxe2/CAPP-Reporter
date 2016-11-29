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
	    		if(course.substring(0,1).compare("!") == 0)
	    			options.push_front(course);
	    		else
	    			options.push_back(course);
	    		temp=temp.substr(a+2);
	    		a = temp.find("||");
	    	}
	    	if(temp.substring(0,1).compare("!") == 0)
	    		options.push_front(temp);
	    	else
	    		options.push_back(temp);
	    	reqs.push_back(options);
    	}
    }
    else
    	cout << "error";
	inFile.close();
}

bool special_compare(string &req, set<string> &classes, vector<string> &unacceptable){
	//Iterate through set checking for a match
	for(set<string>::iterator itr = classes.begin(), itr != classes.end(); ++itr){
		int courseNum = stoi(*itr.substring(5));
		int reqNum = stoi(req.substring(5,9));
		if(courseNum >= reqNum  && unacceptable.find(*itr) == unacceptable.end() )
			return true;
	}
	return false;
}

void compare_courses(vector<vector<string> > &reqs, set<string> &classes, vector<int> &needed){
	for(int i = 0; i < reqs.size(); ++i){
		//Find unacceptable course
		set<string> unacceptable;
		int before = 0;
		while(reqs[i][before].substring(0,1).compare("!") == 0){
			unacceptable.insert(reqs[i][before].subtring(1));
			before++;
		}

		bool satisfied = false;
		for(int j = before; j < reqs.size(); ++j){
			string temp = reqs[i][j];
			//Range(CSCI-4000+)
			if(temp.subtring(0,1).compare("*") == 0){
				satisfied = special_compare(temp, classes, unacceptable);
			}
			//Regular courses
			else{
				if(classes.find(temp) != classes.end()){
					satisfied = true;
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

int main(int n, char* args[]){
	if(args[1]==NULL || args[2]==NULL || args[3]==NULL){
		cerr << "One or more arguments is NULL." << endl;
		return 0;
	}

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