

#ifndef ENVIRONMENT_DATA
#define ENVIRONTMENT_DATA


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


class EnvironmentData {
  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:
    
    public:
        vector<OtherTank> otherTanks;
        vector<Obstacle> obstacles;
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







