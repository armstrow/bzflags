#ifndef ROBOCONTROLLER
#define ROBOCONTROLLER

#include "BZFSCommunicator.h"
//#include "EnvironmentData.h"
#include "Robot.h"
#include "MyTank.h"

#include <vector>
#include <string>
#include <pthread.h>

class RobotController {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */


    private:
        vector<pthread_t> robotThreads;
        vector<Robot *> robotList; 
	int decoy;
	pthread_mutex_t socket_lock;
    public:
        BZFSCommunicator bzfsComm;
        EnvironmentData env;
        string myColor;



    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */


    public:
        RobotController(string server, int port);
	    void DummyRobot(int index);
        void PlayGame();
        bool shoot(int tankIndex);
        bool speed(int tankIndex, double speed);
        bool angvel(int tankIndex, double angVel);
        bool accelx(int tankIndex, double accelX);
        bool accely(int tankIndex, double accelY);

	    //Information Requests
        //  +---> these are now found in the BZFSCommunicator

    private:
        //static void* MakeRobot(void *robotArgs);
        void LoopAction();
        void InitRobots();
	    bool SendBoolMessage(string msg);
        void UpdateEnvironment();
        void InitEnvironment();
        void ControlRobots();
};



class MakeRobotArgs {
    public:
        RobotController *thisRC;
        MyTank *meTank;
        EnvironmentData *env;
	vector<Robot*> *robotList;

        MakeRobotArgs(RobotController *thisRC, MyTank *meTank, EnvironmentData *env, vector<Robot*> *rl): thisRC(thisRC),meTank(meTank), env(env), robotList(rl){}
};




#endif
