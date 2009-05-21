

#ifndef ENVIRONMENT_DATA
#define ENVIRONMENT_DATA


#include <vector>
#include <iostream>
#include <string.h>
#include <string>
#include <pthread.h>

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
    	 pthread_mutex_t otherTanks_lock;
	    pthread_mutex_t obstacles_lock;
    
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
        vector<Obstacle> getObstacles() {
        		pthread_mutex_lock(&obstacles_lock);
        		vector<Obstacle> obst = obstacles;
        		pthread_mutex_unlock(&obstacles_lock);
        		return obst;
        };
        vector<OtherTank> getOtherTanks() {
        	   pthread_mutex_lock(&otherTanks_lock);
        		vector<OtherTank> ot = otherTanks;
        		pthread_mutex_unlock(&otherTanks_lock);
        		return ot;
        	};
        
};







#endif







