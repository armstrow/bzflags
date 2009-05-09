


#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <stdlib.h>

#include "Base.h"

using namespace std;




/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



Base::Base(vector<string> data) {
    if(data.at(0) != "base")
        throw "Invalid data for Base";
	 this->color = data.at(1);
    int i = 2;
    while(1 == 1) {
        if(i == static_cast<int>(data.size()))
            break;

        BasePoint corner;
        corner.x = atof(data.at(i).c_str());
        i++;
        corner.y = atof(data.at(i).c_str());
        this->corners.push_back(corner);
        i++;
    }
}




/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





