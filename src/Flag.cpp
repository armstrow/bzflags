


#include "Flag.h"
#include <stdlib.h>
#include <sstream>

using namespace std;




/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



Flag::Flag(vector<string> data) {
    SetData(data);
}
void Flag::SetData(vector<string> data) {
  if (data.at(0) != "flag")
    throw "Invalid data for flag";

  this->color = data.at(1);
  this->poss_color = data.at(2);
  this->pos[0] = atof(data.at(3).c_str());
  this->pos[1] = atof(data.at(4).c_str());
}
string Flag::ToString() {
  stringstream sout;
  sout << this->color << " team possesed by: " << this->poss_color << " at (" << this->pos[0] << "," << this->pos[1] << ")";
  string result = sout.str();
  return result;
}




/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





