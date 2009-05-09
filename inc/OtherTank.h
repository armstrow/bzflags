

#ifndef OTHER_TANK
#define OTHER_TANK


#include <vector>
#include <string.h>
#include <string>
#include <iostream>

using namespace std;



class OtherTank {
  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:

    public:
        string callsign;
        string status;
        string color;
        string flag;
        float x;
        float y;
        float angle;

   
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    private:

    public:
        OtherTank(vector<string> data);
        string ToString();
 
};





#endif









