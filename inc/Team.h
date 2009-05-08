

#ifndef TEAM
#define TEAM

#include <string.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;


class Team {
  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:
        

    public:
        string color;
        int count;
   
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    private:

    public:
        Team(vector<string> data);
        string ToString();
 
};





#endif









