

#include "Team.h"
#include "RobotController.h"
#include "Constant.h"
#include <pthread.h>

#include <vector>

void *MakeRobot(void *args);

//------------------------------------------------------
RobotController::RobotController(string server, int port) {
	if (bzfsComm.Connect(server, port) == 0) {
        cout << "INITING ENV!" << endl;
		InitEnvironment();
	}
	//pthread_mutex_init(&socket_lock, NULL);
}



//------------------------------------------------------
void RobotController::PlayGame() {
    InitRobots();
    LoopAction();
}
//------------------------------------------------------
void RobotController::InitRobots() {
    cout << "number of mytanks: " << env.myTanks.size() << endl;
    for(int i = 0; i < env.myTanks.size(); i++) {
        pthread_t *newThread = new pthread_t();
        //robotThreads.push_back(newThread);

        MyTank *currTank = &env.myTanks.at(i);
        MakeRobotArgs args(this, currTank, &env);

        pthread_create(newThread, NULL, &MakeRobot, (void *)&args);
    }
}
//------------------------------------------------------
void* MakeRobot(void *passedArgs) {
    //cout << "IN NEW THREAD: MAKE-ROBOT!!" << endl;
    //sleep(2);
    MakeRobotArgs *args = (MakeRobotArgs *)passedArgs;
    Robot currBot(args->meTank, &args->thisRC->bzfsComm, args->env);
    currBot.BeAlive();
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
    cout << "RC --> INIT OBSTS" << endl;
    bzfsComm.get_obstacles(&env.obstacles);
    cout << "RC --> INIT TEAMS" << endl;
    bzfsComm.get_teams(&env.teams);
    cout << "RC --> INIT CONSTANTS" << endl;
    bzfsComm.get_constants(&env.constants);
    for(int i = 0; i < env.constants.size(); i++) {
        Constant currConstant = env.constants.at(i);
        if(currConstant.name == "team") {
            this->myColor = currConstant.value;
        }
    }
    bzfsComm.get_bases(&env.bases);
    bzfsComm.get_mytanks(&env.myTanks);
}
//------------------------------------------------------
void RobotController::UpdateEnvironment() {
    bzfsComm.get_shots(&env.shots);
    //cout << "RC --> GOT SHOTS" << endl;
    bzfsComm.get_othertanks(&env.otherTanks);
    //cout << "RC --> GOT OTHER TANKS" << endl;
    bzfsComm.get_mytanks(&env.myTanks);    
    //cout << "RC --> GOT MY TANKS" << endl;
    bzfsComm.get_flags(&env.flags);
    //cout << "RC --> GOT FLAGS" << endl;
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

