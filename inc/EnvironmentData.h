

#ifndef ENVIRONMENT_DATA
#define ENVIRONMENT_DATA


#include <vector>
#include <iostream>
#include <string.h>
#include <string>

#include "OtherTank.h"
#include "Obstacle.h"
#include "Shot.h"
#include "Flag.h"
#include "Constant.h"
#include "Team.h"
#include "MyTank.h"
#include "Base.h"


class EnvironmentData {
  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:
    
    public:
        vector<OtherTank> otherTanks;
        vector<MyTank> myTanks;
        vector<Obstacle> obstacles;
        vector<Base> bases;
        vector<Shot> shots;
        vector<Flag> flags;
        vector<Constant> constants;
        vector<Team> teams;
 


    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    private:

    public:
        EnvironmentData();
};







#endif






