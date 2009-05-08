

#include "Team.h"
#include "RobotController.h"

#include <vector>


//------------------------------------------------------
RobotController::RobotController(string server, int port) {
    bzfsComm.Connect(server, port);

    InitEnvironment();
    LoopAction();

    //Start Dummy
    if (speed(0, 1))
	    cout << "Moving tank Success!!" << endl;
    else
	    cout << "Moving tank Failure =(" << endl;
}
//------------------------------------------------------
void RobotController::LoopAction() {
    while(1 == 1) {
        cout << "------------------------------------------" << endl;
        cout << "------------------------------------------" << endl;
        UpdateEnvironment();
        vector<Team> teams;
        bzfsComm.get_teams(&teams);
        cout << "num teams: " << teams.size() << endl;
        for(int i = 0; i < teams.size(); i++) {
            cout << teams.at(i).ToString() << endl;
        }
        sleep(2);
    }
}
//------------------------------------------------------
void RobotController::InitEnvironment() {
    bzfsComm.get_teams(&env.teams);
}
//------------------------------------------------------
void RobotController::UpdateEnvironment() {
    
}
//------------------------------------------------------


/****************************
*   Commands:
****************************/
bool RobotController::SendBoolMessage(string msg) {
	vector <string> reply = bzfsComm.SendMessage(msg);
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

