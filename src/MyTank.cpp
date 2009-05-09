


#include <iostream>
#include <string>
#include <string.h>
#include <cstdlib>
#include <sstream>

#include "MyTank.h"

using namespace std;




/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



MyTank::MyTank(vector<string> data) {
    SetData(data);
}
void MyTank::SetData(vector<string> data) {
	  if (data.at(0) != "mytank")
    		throw "Invalid data for MyTanks";
    		
	    this->index=atoi(data.at(1).c_str());
	    this->callsign=data.at(2);
	    this->status=data.at(3);
	    this->shots_avail=atoi(data.at(4).c_str());
	    this->time_to_reload=atof(data.at(5).c_str());
	    this->flag=data.at(6);
	    this->pos[0]=atof(data.at(7).c_str());
	    this->pos[1]=atof(data.at(8).c_str());
	    this->angle=atof(data.at(9).c_str());
	    this->velocity[0]=atof(data.at(10).c_str());
	    this->velocity[1]=atof(data.at(11).c_str());
	    this->angvel=atof(data.at(12).c_str());
}
string MyTank::ToString() {
    stringstream sout;
    sout << "tank #" << this->index << " status: " << this->status << " angvel: " << this->angvel << " vx: " << this->velocity[0] << " vy: " << this->velocity[1];
    string result = sout.str();
    return result;
}



/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





