


#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>

#include "OtherTank.h"

using namespace std;




/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



OtherTank::OtherTank(vector<string> data) {
    if(data.at(0) != "othertank")
        throw "Invalid data for OtherTank";
    
    this->callsign = data.at(1);
    this->color = data.at(2);
    this->status = data.at(3);
    this->flag = data.at(4);
    this->x = atof(data.at(5).c_str());
    this->y = atof(data.at(6).c_str());
    this->angle = atof(data.at(7).c_str());
}
string OtherTank::ToString() {
    string result = "";
    return result;
}



/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





