#ifndef ROBOCONTROLLER
#define ROBOCONTROLLER

#include "Robot.h"
#include "BZFSCommunicator.h"
#include "EnvironmentData.h"

#include <vector>
#include <string>
#include <pthread.h>

class RobotController {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */


    private:
        vector<Robot> robotList; 
        BZFSCommunicator bzfsComm;
	pthread_mutex_t socket_lock;
    public:
        EnvironmentData env;



    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */


    public:
        RobotController(string server, int port);
	void DummyRobot(int index);
	    //Commands
    	bool shoot(int index);
    	bool speed(int index, double value);
    	bool angvel(int index, double value);
    	bool accelx(int index, double value);
    	bool accely(int index, double value);
        void LoopAction();

	    //Information Requests
        //  +---> these are now found in the BZFSCommunicator

    private:
	bool SendBoolMessage(string msg);
        void UpdateEnvironment();
        void InitEnvironment();
};




#endif
