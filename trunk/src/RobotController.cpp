

#include "Team.h"
#include "RobotController.h"
#include "Constant.h"
#include "KalmenFilter.h"
#include <math.h>
#include <pthread.h>

#include <vector>

//vector<Robot *> robotList;

void *MakeRobot(void *args);
void *MakeSniper(void *args);
void *MakeDecoy(void *args);
bool canEnter = true;

//------------------------------------------------------
RobotController::RobotController(string server, int port, int robotNum, string robotStartType) {
    decoy = -1;
    loopCount = 0;
    this->robotNum = robotNum;
    this->robotStartType = robotStartType;
	if (bzfsComm.Connect(server, port) == 0) {
        cout << "INITING ENV!" << endl;
		InitEnvironment();
	} else {
        cout << "there was a problem connec ting to the bzrobots server" << endl;
    }
}
//------------------------------------------------------
void RobotController::PlayGame() {
    InitRobots();
    LoopAction();
}
//------------------------------------------------------
void RobotController::InitRobots() {
    cout << "Initing robots" << endl;
    cout << "ENV SIZE: " << env.myTanks.size();
	for (int i = 0; i < env.myTanks.size(); i++) {
		MyTank *currTank = &env.myTanks.at(i);
		Robot *currBot = new Robot(currTank, &bzfsComm, &env, robotStartType);
		currBot->SwitchTo(robotStartType);//CP_CONST_XY_VEL);
		robotList.push_back(currBot);
		cout << "add bot #" << robotNum << endl;
	}
}
//------------------------------------------------------
void RobotController::LoopAction() {
    while(1 == 1) {
        UpdateEnvironment();
        ControlRobots();
        loopCount++;
    }
}
//------------------------------------------------------
void RobotController::InitEnvironment() {
    bzfsComm.get_obstacles(&env.obstacles);
    bzfsComm.get_teams(&env.teams);
    bzfsComm.get_constants(&env.constants);
    bzfsComm.get_bases(&env.bases);
    bzfsComm.get_mytanks(&env.myTanks);
    bzfsComm.get_flags(&env.flags);
    bzfsComm.get_othertanks(&env.otherTanks);
}
//------------------------------------------------------
void RobotController::UpdateEnvironment() {
    bzfsComm.get_shots(&env.shots);
    bzfsComm.get_othertanks(&env.otherTanks);
    bzfsComm.get_mytanks(&env.myTanks);
    bzfsComm.get_flags(&env.flags);
}
//------------------------------------------------------
void RobotController::ControlRobots() {
    bool enemyTanksDead = true;
    int numDead = 0;
    float otherX;
    float otherY;
    for(int i = 0; i < env.otherTanks.size(); i++) {
        OtherTank currTank = env.otherTanks.at(i);
        if(currTank.status == "normal"){
            otherX = currTank.x;
            otherY = currTank.y;
        } else if(currTank.status == "dead") {
            numDead++;
        }
    }

    enemyTanksDead = numDead == env.otherTanks.size();

    int switchDist = 410;

    for(int i = 0; i < robotList.size(); i++) {
        cout << i << "  ---------------------------------------------------" << endl;
        Robot *currRobot = robotList.at(i);
		//float meX = currRobot->meTank->pos[0];
        //float meY = currRobot->meTank->pos[1];
        //float dist = sqrt((meX - otherX)*(meX - otherX) + (meY - otherY)*(meY - otherY));
        /*
        if(enemyTanksDead)
            currRobot->SwitchTo(TRAVEL);
        else if(dist < switchDist && !enemyTanksDead) {
            switchDist = 350;
            cout << "SHOULD HAVE SWITCHED!! dist: " << dist << endl;
            if (decoy == -1 || decoy == i) {
                currRobot->SwitchTo(DECOY);
                decoy = i;
            } else {
                currRobot->SwitchTo(SNIPER);
            }
        }
        */
        //if((i % 2 == 0) || (i % 2 == 1 && loopCount > 100))
        currRobot->Update();
    }
}
//------------------------------------------------------


