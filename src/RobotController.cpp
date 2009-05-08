

#include "RobotController.h"


//------------------------------------------------------
RobotController::RobotController(string server, int port) {
    bzfsComm.Connect(server, port);
    //Start Dummy
    if (speed(0, 1))
	cout << "Moving tank Success!!" << endl;
    else
	cout << "Moving tank Failure =(" << endl;
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

/******************************
* Information Requests
******************************/
bool RobotController::get_teams(){//vector <team_t> *AllTeams) {
//Request a list of teams.
	cout << bzfsComm.SendMessage("teams").at(0);
/*
	SendLine("teams");
	ReadAck();
	vector <string> v=ReadArr();
	if(v.at(0)!="begin") {
		return false;
	}
	v.clear();
	v=ReadArr();
	int i=0;
	while(v.at(0)=="team") {
		team_t MyTeam;
		MyTeam.color=v.at(1);
		MyTeam.count=atoi(v.at(2).c_str());
		MyTeam.base_corner[0][0]=atof(v.at(3).c_str());
		MyTeam.base_corner[0][1]=atof(v.at(4).c_str());
		MyTeam.base_corner[1][0]=atof(v.at(5).c_str());
		MyTeam.base_corner[1][1]=atof(v.at(6).c_str());
		MyTeam.base_corner[2][0]=atof(v.at(7).c_str());
		MyTeam.base_corner[2][1]=atof(v.at(8).c_str());
		MyTeam.base_corner[3][0]=atof(v.at(9).c_str());
		MyTeam.base_corner[3][1]=atof(v.at(10).c_str());
		AllTeams->push_back(MyTeam);
		v.clear();
		v=ReadArr();
		i++;
	}
	if(v.at(0)!="end") {
		return false;
	}
	return true;
}

	bool get_obstacles(vector <obstacle_t> *AllObstacles) {
		// Request a list of obstacles.
		SendLine("obstacles");
		ReadAck();
		vector <string> v=ReadArr();
		if(v.at(0)!="begin") {
			return false;
		}
		v.clear();
		v=ReadArr();
		int i=0;
		while(v.at(0)=="obstacle") {
			obstacle_t MyObstacle;
			int j=0;
			while(j+2<(int)v.size()) {
				MyObstacle.o_corner[j/2][0]=atof(v.at(j+1).c_str());
				MyObstacle.o_corner[j/2][1]=atof(v.at(j+2).c_str());
				j=j+2;
			}
			AllObstacles->push_back(MyObstacle);
			v.clear();
			v=ReadArr();
			i++;
		}
		if(v.at(0)!="end") {
			return false;
		}
		return true;
	}

	bool get_flags(vector <flag_t> *AllFlags) {
		// Request a list of flags.
		SendLine("flags");
		ReadAck();
		vector <string> v=ReadArr();
		if(v.at(0)!="begin") {
			return false;
		}
		v.clear();
		v=ReadArr();
		int i=0;
		while(v.at(0)=="flag") {
			flag_t MyFlag;
			MyFlag.color=v.at(1);
			MyFlag.poss_color=v.at(2);
			MyFlag.pos[0]=atof(v.at(3).c_str());
			MyFlag.pos[1]=atof(v.at(4).c_str());
			AllFlags->push_back(MyFlag);
			v.clear();
			v=ReadArr();
			i++;
		}
		if(v.at(0)!="end") {
			return false;
		}
		return true;
	}

	bool get_shots(vector <shot_t> *AllShots) {
		// Request a list of shots.
		SendLine("shots");
		ReadAck();
		vector <string> v=ReadArr();
		if(v.at(0)!="begin") {
			return false;
		}
		v.clear();
		v=ReadArr();
		int i=0;
		while(v.at(0)=="shot") {
			shot_t MyShot;
			MyShot.pos[0]=atof(v.at(1).c_str());
			MyShot.pos[1]=atof(v.at(2).c_str());
			MyShot.velocity[0]=atof(v.at(3).c_str());
			MyShot.velocity[1]=atof(v.at(4).c_str());
			AllShots->push_back(MyShot);
			v.clear();
			v=ReadArr();
			i++;
		}
		if(v.at(0)!="end") {
			return false;
		}
		return true;
	}

	bool get_mytanks(vector <tank_t> *AllMyTanks) {
		// Request a list of our robots.
		SendLine("mytanks");
		ReadAck();
		vector <string> v=ReadArr();
		if(v.at(0)!="begin") {
			return false;
		}
		v.clear();
		v=ReadArr();
		int i=0;
		while(v.at(0)=="mytank") {
			tank_t MyTank;
			MyTank.index=atoi(v.at(1).c_str());
			MyTank.callsign=v.at(2);
			MyTank.status=v.at(3);
			MyTank.shots_avail=atoi(v.at(4).c_str());
			MyTank.time_to_reload=atof(v.at(5).c_str());
			MyTank.flag=v.at(6);
			MyTank.pos[0]=atof(v.at(7).c_str());
			MyTank.pos[1]=atof(v.at(8).c_str());
			MyTank.angle=atof(v.at(9).c_str());
			MyTank.velocity[0]=atof(v.at(10).c_str());
			MyTank.velocity[1]=atof(v.at(11).c_str());
			MyTank.angvel=atof(v.at(12).c_str());
			AllMyTanks->push_back(MyTank);
			v.clear();
			v=ReadArr();
			i++;
		}
		if(v.at(0)!="end") {
			if(debug) cout << v.at(0) << endl;
			return false;
		}
		return true;
	}

	bool get_othertanks(vector <otank_t> *AllOtherTanks) {
		// Request a list of tanks that aren't our robots.
		SendLine("othertanks");
		ReadAck();
		vector <string> v=ReadArr();
		if(v.at(0)!="begin") {
			return false;
		}
		v.clear();
		v=ReadArr();
		int i=0;
		while(v.at(0)=="othertank") {
			otank_t OtherTank;
			OtherTank.callsign=v.at(1);
			OtherTank.color=v.at(2);
			OtherTank.status=v.at(3);
			OtherTank.flag=v.at(4);
			OtherTank.pos[0]=atof(v.at(5).c_str());
			OtherTank.pos[1]=atof(v.at(6).c_str());
			OtherTank.angle=atof(v.at(7).c_str());
			AllOtherTanks->push_back(OtherTank);
			v.clear();
			v=ReadArr();
			i++;
		}
		if(v.at(0)!="end") {
			return false;
		}
		return true;
	}

	bool get_constants(vector <constant_t> *AllConstants) {
		// Request a dictionary of game constants.
		SendLine("constants");
		ReadAck();
		vector <string> v=ReadArr();
		if(v.at(0)!="begin") {
			return false;
		}
		v.clear();
		v=ReadArr();
		int i=0;
		while(v.at(0)=="constant") {
			constant_t MyConstant;
			MyConstant.name=v.at(1);
			MyConstant.value=v.at(2);
			AllConstants->push_back(MyConstant);
			v.clear();
			v=ReadArr();
			i++;
		}
		if(v.at(0)!="end") {
			return false;
		}
		return true;
	}
*/
}


