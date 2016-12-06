#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>
#include <map>

//The name of the program
#define APPLICATION_NAME "CAPP Reporter"

//Helpful typedefs
typedef unsigned int uint;
typedef std::map<std::string, std::string> algoMap;

//Prints Msg to stderr and exits
void Err(const char * Msg);

//Perrors, prints Msg to stderr, and exits
void pErr(const char * Msg);

//Asserts a value is true, Err(Msg) if not.
void Assert(const bool b, const char * Msg);

#endif
