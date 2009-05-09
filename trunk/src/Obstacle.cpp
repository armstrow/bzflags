


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
    while(index < data.size()) {
        Point corner;
        float x = atof(data.at(index).c_str());
        float y = atof(data.at(index + 1).c_str());
        cout << "X: " << x << endl;
        cout << "Y: " << y << endl;
        corner.x = x;
        corner.y = y;
        this->corners.push_back(corner);
        index += 2;
    }
}




/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





