

#include "Team.h"
#include "RobotController.h"
#include "Constant.h"
#include <math.h>
#include <pthread.h>

#include <vector>

//vector<Robot *> robotList;

void *MakeRobot(void *args);
void *MakeSniper(void *args);
void *MakeDecoy(void *args);
bool canEnter = true;

//------------------------------------------------------
RobotController::RobotController(string server, int port) {
    decoy = -1;
	if (bzfsComm.Connect(server, port) == 0) {
        cout << "INITING ENV!" << endl;
		InitEnvironment();
	}
}
//------------------------------------------------------
void RobotController::PlayGame() {
    InitRobots();
    LoopAction();
}
//------------------------------------------------------
void RobotController::InitRobots() {
    for(int i = 0; i < env.myTanks.size(); i++) {
        cout << i << "   -------------------------------" << endl;
        MyTank *currTank = &env.myTanks.at(i);
        Robot *currBot = new Robot(currTank, &bzfsComm, &env);
        currBot->SwitchTo(TRAVEL);
        robotList.push_back(currBot);
        cout << "add bot #" << i << endl;
    }
}
//------------------------------------------------------
void RobotController::LoopAction() {
    while(1 == 1) {
        UpdateEnvironment();
        ControlRobots();
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
    float otherX;
    float otherY;
    float dist = 9999999;
    for(int i = 0; i < env.otherTanks.size(); i++) {
        OtherTank currTank = env.otherTanks.at(i);
        if(currTank.status == "normal"){
            enemyTanksDead = false;
            otherX = currTank.x;
            otherY = currTank.y;
        }
    }

    int switchDist = 390;

    for(int i = 0; i < robotList.size(); i++) {
        cout << i << "  ---------------------------------------------------" << endl;
        Robot *currRobot = robotList.at(i);
        float meX = currRobot->meTank->pos[0];
        float meY = currRobot->meTank->pos[1];
        float dist = sqrt((meX - otherX)*(meX - otherX) - (meY - otherY)*(meY - otherY));
        if(enemyTanksDead)
            currRobot->SwitchTo(TRAVEL);
        else if(dist < 150 && !enemyTanksDead) {
            if (decoy == -1 || decoy == i) {
                currRobot->SwitchTo(DECOY);
                decoy = i;
            } else {
                currRobot->SwitchTo(SNIPER);
            }
        }
        currRobot->Update();
    }
}
//------------------------------------------------------


