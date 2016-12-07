#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>

//For ease of reading
typedef map<string, int> courseMap;
typedef vector<string> commentVector;
typedef map<string, string> outputMap;
typedef vector<vector<string> > reqsVector;
                                           
//Forward declarations
void parse_reqs(const string &fName, reqsVector& majorReqs, commentVector& 
                majorComments, reqsVector& hassReqs, commentVector& hassComments);
void parse_reqs(reqsVector& reqs, const string &fName);
bool special_compare(const string &req, courseMap& classes, 
                        const set<string> &unacceptable, bool noRepeat);
int free_electives(courseMap& classes, const string &credits);
int concentration_compare(const string &initConcentration, courseMap& classes_credits);
void compare_courses(reqsVector& reqs, courseMap& classes, 
                        vector<int> &needed, bool noRepeat, int numCourses);
void compare_courses(reqsVector& reqs, courseMap& classes, vector<int> &needed);
pair<outputMap*, outputMap* > runAlgo(const string &req_file, courseMap courses);

