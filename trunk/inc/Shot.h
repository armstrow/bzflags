

#ifndef SHOT 
#define SHOT

#include <string>
#include <string.h>
#include <vector>
#include <iostream>

using namespace std;

class Shot {
  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:

    public:
	float pos[2];
	float velocity[2];
   
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    private:

    public:
	Shot(vector<string> data);
	void SetData(vector<string> data);
	string ToString();
 
};





#endif









