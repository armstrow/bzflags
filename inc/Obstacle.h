

#ifndef OBSTACLE 
#define OBSTACLE

#include <vector>
#include <string.h>
#include <string>

using namespace std;

struct Point {
    float x;
    float y;
};

class Obstacle {
  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:

    public:
        vector<Point> corners;
   
    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    private:

    public:
        Obstacle(vector<string> data);
 
};





#endif









