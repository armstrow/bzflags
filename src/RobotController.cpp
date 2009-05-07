

#include "RobotController.h"


//------------------------------------------------------
RobotController::RobotController(string server, int port) {
    bzfsComm.Connect(server, port);
}
//------------------------------------------------------



