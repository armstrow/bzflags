#ifndef ROBOCONTROLLER
#define ROBOCONTROLLER

#include "Robot.h"
#include "BZFSCommunicator.h"
#include <vector>

class RobotController {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */


    private:
        vector<Robot> robotList; 
        BZFSCommunicator bzfsComm;



    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */


    public:
        RobotController(string server, int port);

    private:

};




#endif
