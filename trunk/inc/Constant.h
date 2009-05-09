

#ifndef CONSTANT
#define CONSTANT

#include <string>
#include <string.h>
#include <vector>

using namespace std;

class Constant {
  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:

    public:
    	string name;
    	string value;
        
   
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    private:

    public:
    	Constant(vector<string> data);
        string ToString();
 
};





#endif









