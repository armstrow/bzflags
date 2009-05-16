


#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <stdlib.h>
#include <math.h>

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

    //Check accuracy of corners
    Point bl = corners.at(0);
    Point br = corners.at(1);
    Point tr = corners.at(2);
    Point tl = corners.at(3);
    if (floor(bl.x + .5) != floor(tr.x + .5)) { //ignore correction if box is tipped

    if (br.x != tr.x) {
	if (abs(tr.x - tl.x) < abs(br.x - bl.x))
		corners.at(1).x = tr.x;
	else
		corners.at(2).x = br.x;
    }
    if (bl.x != tl.x) {
	if (abs(tl.x - tr.x) < abs(bl.x - br.x))
		corners.at(0).x = tl.x;
	else
		corners.at(3).x = bl.x;
    }
    if (br.y != bl.y) {
	if (abs(br.y - tr.y) < abs(bl.y - tl.y))
		corners.at(0).y = br.y;
	else
		corners.at(1).y = bl.y;
    }
    if (tr.y != tl.y) {
	if (abs(tr.y - br.y) < abs(tl.y - bl.y))
		corners.at(3).y = tr.y;
	else
		corners.at(2).y = tl.y;   
    } 
    }   
}




/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */





