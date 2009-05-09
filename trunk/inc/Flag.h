

#ifndef FLAG 
#define FLAG


#include <iostream>
#include <string>
#include <string.h>
#include <vector>

class Flag {
  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:

    public:

	string color;
	string poss_color;
	float pos[2];
	
	
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    private:

    public:
        Flag(vector<string> data);
	string toString();
 
};





#endif









