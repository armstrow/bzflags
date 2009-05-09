

#ifndef MY_TANK 
#define MY_TANK

#include <string>
#include <vector>

using namespace std;

class MyTank {
  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:

    public:
	int index;
	string callsign;
	string status;
	int shots_avail;
	int time_to_reload;
	string flag;
	float pos[2];
	float angle;
	float velocity[2];
	float angvel;
   
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    private:

    public:
        MyTank(vector<string> data);
        void SetData(vector<string> data);
        string ToString();
 
};





#endif









