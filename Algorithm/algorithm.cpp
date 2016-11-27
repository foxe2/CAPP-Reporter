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
void parse_major_reqs(vector<vector<string> > &reqs, string f_name){
	ifstream instr(f_name);
	if (instr.is_open()){
		string temp;
    	while(getline(instr,temp)){
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

int main(int n, char* args[]){
	vector<vector<string> > reqs;
	if(args[1] != NULL){
		string f_name = args[1];
		parse_major_reqs(reqs, f_name);
	}
	for(int i = 0; i < reqs.size(); ++i){
		for(int j = 0; j < reqs[i].size(); ++j)
			cout << reqs[i][j] << " ";
		cout << endl;
	}
	//parse_major_reqs(reqs);
}