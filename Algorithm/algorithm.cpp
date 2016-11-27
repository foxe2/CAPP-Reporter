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

using namespace std;

//Takes in a datastructure which it will add classes to
void parse_reqs(vector<vector<string> > &reqs, string &f_name){
	ifstream instr(f_name);
	if (instr.is_open()){
		string temp;
    	while(getline(instr,temp)){
		    if(temp.find("//") != -1)
		    	continue;
		    vector<string> options;
	    	int a = temp.find("||");
	    	while(a != -1){
	    		options.push_back(temp.substr(0,a-1));
	    		temp=temp.substr(a+2);
	    		a = temp.find("||");
	    	}
	    	options.push_back(temp);
	    	reqs.push_back(options);
    	}
    }
    else
    	cout << "error";
	instr.close();
}

void compare_courses(vector<vector<string> > &reqs, Set<string> &classes, vector<int> &needed){
	for(int i = 0; i < reqs.size(); ++i){
		bool satisfied = false;
		for(int j = 0; j < reqs.size(); ++j){
			if(classes.find(reqs[i][j]) != classes.end()){
				satisfied = true;
				classes.erase(reqs[i][j]);
				break;
			}
		}
		if(!satisfied)
			needed.push_back(i);
	}
}

int main(int n, char* args[]){
	vector<vector<string> > reqs;
	if(args[1] != NULL){
		string f_name = args[1];
		parse_reqs(reqs, f_name);
	}
	for(int i = 0; i < reqs.size(); ++i){
		for(int j = 0; j < reqs[i].size(); ++j)
			cout << reqs[i][j] << " ";
		cout << endl;
	}
	//parse_major_reqs(reqs);
}