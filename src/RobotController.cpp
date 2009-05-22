

#include "Team.h"
#include "RobotController.h"
#include "Constant.h"
#include <pthread.h>

#include <vector>

//vector<Robot *> robotList;

void *MakeRobot(void *args);
void *MakeSniper(void *args);
void *MakeDecoy(void *args);

pthread_mutex_t botList_lock;
bool canEnter = true;

//------------------------------------------------------
RobotController::RobotController(string server, int port) {
	if (bzfsComm.Connect(server, port) == 0) {
        cout << "INITING ENV!" << endl;
		InitEnvironment();
	}
	//pthread_mutex_init(&botList_lock, NULL);
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
        pthread_t *newThread = new pthread_t();
        //robotThreads.push_back(newThread);
        MyTank *currTank = &env.myTanks.at(i);
        MakeRobotArgs args(this, currTank, &env, &robotList);
	//	while(!canEnter) usleep(100);
	pthread_mutex_lock(&botList_lock);
	//canEnter = false;
	
	//if(sniper) 
            pthread_create(newThread, NULL, &MakeRobot, (void *)&args);
        //else
        //    pthread_create(newThread, NULL, &MakeRobot, (void *)&args);
        //sniper = true;
    }
}
//------------------------------------------------------
void* MakeRobot(void *passedArgs) {
    //cout << "IN NEW THREAD: MAKE-ROBOT!!" << endl;
    //sleep(2);
    MakeRobotArgs *args = (MakeRobotArgs *)passedArgs;
    Robot currBot(args->meTank, &args->thisRC->bzfsComm, args->env);
    //vector<Robot *> *roboList = args->robotList;

    args->robotList->push_back(&currBot);
    pthread_mutex_unlock(&botList_lock);
    canEnter = true;
    currBot.BeAlive(TRAVEL);
}
void* MakeSniper(void *passedArgs) {
    //cout << "IN NEW THREAD: MAKE-ROBOT!!" << endl;
    //sleep(2);
    MakeRobotArgs *args = (MakeRobotArgs *)passedArgs;
    Robot currBot(args->meTank, &args->thisRC->bzfsComm, args->env);
    vector<Robot *> *roboList = args->robotList;
    currBot.BeAlive(SNIPER);
    roboList->push_back(&currBot);
}
void* MakeDecoy(void *passedArgs) {
    //cout << "IN NEW THREAD: MAKE-ROBOT!!" << endl;
    //sleep(2);
    MakeRobotArgs *args = (MakeRobotArgs *)passedArgs;
    Robot currBot(args->meTank, &args->thisRC->bzfsComm, args->env);
    vector<Robot *> *roboList = args->robotList;
    currBot.BeAlive(DECOY);
    roboList->push_back(&currBot);
}
//------------------------------------------------------
void RobotController::LoopAction() {
    while(1 == 1) {
        pthread_mutex_lock(&socket_lock);
        UpdateEnvironment();
        pthread_mutex_unlock(&socket_lock);
        usleep(500);
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

    ControlRobots();
}
//------------------------------------------------------
void RobotController::ControlRobots() {
    //decoys/snipers will always be in pairs
    bool enemyTanksDead = true;
    float otherX;
    for(int i = 0; i < env.otherTanks.size(); i++) {
        OtherTank currTank = env.otherTanks.at(i);
        if(currTank.status == "normal"){
            enemyTanksDead = false;
	    otherX = currTank.x;
	}
    }
    //if(robotList) {
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
	    //}
	    //else if(abs(otherX - currRobot->meTank->pos[0]) < 200 && !enemyTanksDead)
	     // currRobot->SwitchTo(SNIPER);
    	}
    //}
}
//------------------------------------------------------


/****************************
*   Commands:
****************************/

bool RobotController::SendBoolMessage(string msg) {
	pthread_mutex_lock(&socket_lock);
	vector <string> reply = bzfsComm.SendMessage(msg);
	pthread_mutex_unlock(&socket_lock);
	if(reply.at(0) == "ok") {
		return true;
	}
	else {
		return false;
	}
}

bool RobotController::shoot(int index) {
// Perform a shoot request.
	char char_buff[20];
	sprintf(char_buff, " %d", index);	
	string str_buff="shoot";
	str_buff.append(char_buff);
	return SendBoolMessage(str_buff);
}

bool RobotController::speed(int index, double value) {
// Set the desired speed to the specified value.
	char char_buff[20];
	sprintf(char_buff, " %d", index);	
	string str_buff="speed";
	str_buff.append(char_buff);
	sprintf(char_buff, " %f", value);
	str_buff.append(char_buff);
	return SendBoolMessage(str_buff);
}

bool RobotController::angvel(int index, double value) {
// Set the desired angular velocity to the specified value.
	char char_buff[20];
	sprintf(char_buff, " %d", index);	
	string str_buff="angvel";
	str_buff.append(char_buff);
	sprintf(char_buff, " %f", value);
	str_buff.append(char_buff);
	return SendBoolMessage(str_buff);
}

bool RobotController::accelx(int index, double value) {
// Set the desired accelaration in x axis to the specified value in hovertank mode.
	char char_buff[20];
	sprintf(char_buff, " %d", index);	
	string str_buff="accelx";
	str_buff.append(char_buff);
	sprintf(char_buff, " %f", value);
	str_buff.append(char_buff);
	return SendBoolMessage(str_buff);
}

bool RobotController::accely(int index, double value) {
// Set the desired accelaration in x axis to the specified value in hovertank mode.
	char char_buff[20];
	sprintf(char_buff, " %d", index);	
	string str_buff="accely";
	str_buff.append(char_buff);
	sprintf(char_buff, " %f", value);
	str_buff.append(char_buff);
	return SendBoolMessage(str_buff);
}

