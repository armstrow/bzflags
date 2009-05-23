

#include "Team.h"
#include "RobotController.h"
#include "Constant.h"
#include <pthread.h>

#include <vector>

//vector<Robot *> robotList;

void *MakeRobot(void *args);
void *MakeSniper(void *args);
void *MakeDecoy(void *args);
bool canEnter = true;

//------------------------------------------------------
RobotController::RobotController(string server, int port) {
	if (bzfsComm.Connect(server, port) == 0) {
        cout << "INITING ENV!" << endl;
		InitEnvironment();
	}
	decoy = -1;
}
//------------------------------------------------------
void RobotController::PlayGame() {
    InitRobots();
    LoopAction();
}
//------------------------------------------------------
void RobotController::InitRobots() {
    cout << "number of mytanks: " << env.myTanks.size() << endl;
    bool sniper = false;
    for(int i = 0; i < env.myTanks.size(); i++) {
        MyTank *currTank = &env.myTanks.at(i);
        Robot *currBot = new Robot(currTank, &bzfsComm, &env);
        currBot->SwitchTo(TRAVEL);
        robotList.push_back(currBot);
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
    for(int i = 0; i < env.constants.size(); i++) {
        Constant currConstant = env.constants.at(i);
        if(currConstant.name == "team") {
            this->myColor = currConstant.value;
        }
    }
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
        for(int i = 0; i < env.otherTanks.size(); i++) {
                OtherTank currTank = env.otherTanks.at(i);
                if(currTank.status == "normal"){
                        enemyTanksDead = false;
                        otherX = currTank.x;
                }
        }
        cout << "AAAControl robots: " << robotList.size() << endl;
        for(int i = 0; i < robotList.size(); i++) {
                Robot *currRobot = robotList.at(i);
                //currRobot->SwitchTo(TRAVEL);
                cout << "AAATank #" << i << ": xpos: " << currRobot->meTank->pos[0] << endl; 
                //if (!hasDec) cout << "AAAHasDecoy is false" << endl;
                if(enemyTanksDead)
                        currRobot->SwitchTo(TRAVEL);
                else if(abs(otherX - currRobot->meTank->pos[0]) < 200 && !enemyTanksDead) {
                        if (decoy == -1 || decoy == i) {
                                currRobot->SwitchTo(DECOY);
                                decoy = i;
                        }
                        else {
                                currRobot->SwitchTo(SNIPER);
                        }
                }
                currRobot->Update();
        }
}
//------------------------------------------------------


