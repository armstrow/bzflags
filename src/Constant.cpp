


#include <iostream>
#include <string>
#include <string.h>

#include "Constant.h"

using namespace std;




/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



Constant::Constant(vector<string> data) {
	if (data.at(0) != "constant")
		throw "Invalid data for Constant";
	this->name = data.at(1);
	this->value = data.at(2);
}
string Constant::ToString() {
	return "Constant " + this->name + " = " + this->value;
}




/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





