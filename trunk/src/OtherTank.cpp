


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
    SetData(data);
}
void OtherTank::SetData(vector<string> data) {
    if(data.at(0) != "othertank")
        throw "Invalid data for OtherTank";
   
    int index = 0;
    this->callsign = data.at(++index);
    this->color = data.at(++index);
    if(data.size() != 8)
        index--;
    this->status = data.at(++index);
    this->flag = data.at(++index);
    this->x = atof(data.at(++index).c_str());
    this->y = atof(data.at(++index).c_str());
    this->angle = atof(data.at(++index).c_str());
}
string OtherTank::ToString() {
    string result = "";
    return result;
}



/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





