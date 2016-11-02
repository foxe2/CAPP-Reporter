//Helpful typedef
typedef unsigned int uint;

//Prints Msg to stderr and exits
void Err(const char * Msg);

//Perrors, prints Msg to stderr, and exits
void pErr(const char * Msg);

//Asserts a value is true, Err(Msg) if not.
void Assert(const bool b, const char * Msg);
