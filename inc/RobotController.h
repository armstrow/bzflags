#ifndef ROBOCONTROLLER
#define ROBOCONTROLLER

#include "Robot.h"
#include "BZFSCommunicator.h"
#include "EnvironmentData.h"

#include <vector>
#include <string>

class RobotController {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */


    private:
        vector<Robot> robotList; 
        BZFSCommunicator bzfsComm;
        EnvironmentData env;



    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */


    public:
        RobotController(string server, int port);

	    //Commands
    	bool shoot(int index);
    	bool speed(int index, double value);
    	bool angvel(int index, double value);
    	bool accelx(int index, double value);
    	bool accely(int index, double value);

	    //Information Requests
        //  +---> these are now found in the BZFSCommunicator

    private:
	    bool SendBoolMessage(string msg);
        void LoopAction();
        void UpdateEnvironment();
        void InitEnvironment();
};




#endif
