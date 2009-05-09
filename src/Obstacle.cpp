


#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <stdlib.h>

#include "Obstacle.h"

using namespace std;




/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



Obstacle::Obstacle(vector<string> data) {
    if(data.at(0) != "obstacle")
        throw "Invalid data for Obstacle";

    int index = 1;
    while(1 == 1) {
    	if(index = data.size())
            break;

        Point corner;
        corner.x = atof(data.at(index).c_str());
        corner.y = atof(data.at(index + 1).c_str());
        this->corners.push_back(corner);
        index += 2;
    }
}




/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





