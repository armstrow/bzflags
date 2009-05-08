


#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <cstdlib>
#include <sstream>

#include "Team.h"

using namespace std;




/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



//------------------------------------------------------
Team::Team(vector<string> data) {
    if(data.at(0) != "team")
        throw "Invalid data for Team";

    this->color = data.at(1);
    this->count = atoi(data.at(2).c_str());
}
//------------------------------------------------------
string Team::ToString() {
    string result = this->color + " team (";
    stringstream out;
    out << this->count;
    result += out.str() + " tanks)";
    return result;
}
//------------------------------------------------------




/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





