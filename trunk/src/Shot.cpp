


#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <sstream>

#include "Shot.h"

using namespace std;




/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



Shot::Shot(vector<string> data) {
    SetData(data);
}
void Shot::SetData(vector<string> data) {
  if (data.at(0) != "shot")
    throw "Invalid data for Shot";
  this->pos[0] = atof(data.at(1).c_str());
  this->pos[1] = atof(data.at(2).c_str());
  this->velocity[0] = atof(data.at(3).c_str());
  this->velocity[1] = atof(data.at(4).c_str());
}

string Shot::ToString() {
  stringstream sout;
  sout << "shot at (" << pos[0] << "," << pos[1] << ") vel:(" << velocity[0] << "," << velocity[1] << ")";
  string result = sout.str();
  return result;
}



/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





