#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <set>

//For ease of reading
typedef std::map<std::string, int> courseMap;
typedef std::vector<std::string> commentVector;
typedef std::map<std::string, std::string> outputMap;
typedef std::vector<std::vector<std::string> > reqsVector;
                                           
//Create a namespace
namespace Algo {
#if 1
    //Forward declarations
    bool parseReqs(const std::string &fName, reqsVector& majorReqs, commentVector&
            majorComments, reqsVector& hassReqs, commentVector& hassComments);
    void parseConcentrationReqs(reqsVector& reqs, const std::string &fName);
    bool specialCompare(const std::string &req, courseMap& classes,
                const std::set<std::string> &unacceptable, bool noRepeat);
    int freeElectives(courseMap& classes, const std::string &credits);
    bool concentrationCompare(const std::string &initConcentration, courseMap& classes);
    bool compareConcCourses(std::vector<std::string>& reqs, courseMap& classes, bool noRepeat, int numCourses);
    void compareCourses(reqsVector& reqs, courseMap& classes, std::vector<int> &needed);
#endif
    std::pair<outputMap*, outputMap* > runAlgo(const std::string &req_file, courseMap& courses);

}

#endif
