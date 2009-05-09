


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

    int i = 1;
    while(1 == 1) {
        if(i == static_cast<int>(data.size()))
            break;

        Point corner;
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





