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
void parse_major_reqs(vector<vector<string>> &reqs, string f_name){
	ifstream instr;
	instr.open (f_name);
	string temp;
    while(!infile.eof){
	    getline(infile,temp);
	    vector<string> options;
    	options.push_back(temp);
    	reqs.push_back(options);
    	int a = temp.find("||");
    	while(a != -1){
    		options.push_back(temp.substr(0,a-1));
    		temp=temp.substr(a+2);
    	}
    	options.push_back(temp)
    	reqs.push_back();
    }
	infile.close();
}

int main(){
	//DS
	parse_major_reqs();
	parse_major_reqs();
}