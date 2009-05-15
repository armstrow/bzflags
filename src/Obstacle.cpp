


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
    /*cout << "\nreading obstacle";
        cout << " (X: " << data.at(1);
        cout << ", Y: " << data.at(2) << ")";
        cout << " (X: " << data.at(3);
        cout << ", Y: " << data.at(4) << ")";
        cout << " (X: " << data.at(5);
        cout << ", Y: " << data.at(6) << ")";
        cout << " (X: " << data.at(7);
        cout << ", Y: " << data.at(8) << ")";*/
    int index = 1;
    while(index < data.size()) {
        Point corner;
        float x = atof(data.at(index).c_str());
        float y = atof(data.at(index + 1).c_str());
        corner.x = x;
        corner.y = y;
        this->corners.push_back(corner);
        index += 2;
    }
}




/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





