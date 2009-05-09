


#include "Flag.h"

using namespace std;




/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



Flag::Flag(vector<string> data) {
  if (data.at(0) != "flag")
    throw "Invalid data for flag."
  this->color = data.at(1);
  this->poss_color = data.at(2);
  this->pos[0] = atof(data.at(3).c_str());
  this->pos[1] = atof(data.at(4).c_str());
}
string Flag::ToString() {
  return this->color + " team possesed by: " + this->poss_color + " at (" + this->pos[0] + "," + this->pos[1] + ")";
}




/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





