


#include <iostream>
#include <string>
#include <string.h>

#include "Base.h"

using namespace std;




/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



Base::Base(vector<string> data) {
	if (data.at(0) != "base")
		throw "Invalid data for Base";
	this->color = data.at(1);
	this->pos[0] = atof(data.at(2).c_str());
	this->pos[1] = atof(data.at(3).c_str());
}
string Base::toString() {
	return this->color + " Base:(" + this->pos[0] + "," + this->pos[1] + ");
}




/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





