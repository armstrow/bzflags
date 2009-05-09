



#include "RobotController.h"
#include "MyTank.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <cmath>


using namespace std;


void DisplayUsage();
void DisplayBanner();
bool ParseArgs(int argc, char** argv);
bool IsNum(string str);

char* arg0;
bool DEBUG = false;
string SERVER;
string STR_PORT;
int PORT;
RobotController* controller;

void *DummyRobot(void *ptr);

//------------------------------------------------------
int main(int argc, char** argv) {

    DisplayBanner();

    arg0 = argv[0];

    if(argc < 3)
        DisplayUsage();
    else if(!ParseArgs(argc, argv)) {
        exit(0);
    }


    controller = new RobotController(SERVER, PORT);
    pthread_t threadRob;
    int threadResultRob;
    int robNum = 0;
    cout << "Begin Robot Thread" << endl;
    pthread_create(&threadRob, NULL, DummyRobot, (void*)&robNum);

    controller->LoopAction();
    //pthread_join(threadRob, NULL);
//    cout << "End Thread Rob with " << threadResultEnv << endl;*/

} 
  
void *DummyRobot(void *ptr )
{
	sleep(10);
	while (1) {
		int *index;
		index = (int *) ptr;
	      	if (controller->speed(*index, 0.5))
			cout << "tank moving!!!" << endl;
		else
			cout << "This sucks!" << endl;

		MyTank curTank = controller->env.myTanks.at(*index);
		//Wait for collision
		//sleep(5);
//					cout << "**********************************************" << endl;
	//		cout << curTank.velocity[0] << " " << curTank.velocity[1] << endl;
		//	cout << "**********************************************" << endl;
		while (sqrt(curTank.velocity[0]*curTank.velocity[0] +
			         curTank.velocity[1]*curTank.velocity[1]) >= 1) {

			    
			sleep(1);
			}
		//controller->speed(*index, 0);
		//Rotate 60
		float targetAngle = curTank.angle + 1.05;
		if (targetAngle > 3.14)	targetAngle -= 6.28;
		if(controller->angvel(*index, .5))
			cout << "tank turning!!!" << endl;
		else
			cout << "Error, could not turn!  Save me!" << endl;
		//while (curTank.angle <= targetAngle || (targetAngle < 0 && curTank.angle > 0))
		sleep(5);
		controller->angvel(*index, 0);
		cout << "Tank turned successfully!" << endl;
	}
	
}

//------------------------------------------------------
bool ParseArgs(int argc, char** argv) {
    if(argc < 3 || argc > 4)
        return false;
    
    string currArg = "";
    for(int i = 1; i < argc; i++) {
        currArg = argv[i];
        if(currArg != "-d" && SERVER == "")
            SERVER = currArg;
        else if(currArg != "-d" && STR_PORT == "")
            STR_PORT = currArg;
        else if(currArg == "-d")
            DEBUG = true;
    }
    
    if(!IsNum(STR_PORT)) {
        cout << "ERROR:" << endl
             << "    The given port ("+STR_PORT+") is not a number." << endl
             << "    Go away and try again when you know what a number is." << endl;
        return false;
    }

    PORT = atoi(STR_PORT.c_str());

    if(DEBUG) {
        cout << "DEBUG MODE:" << endl
             << "    Server: " << SERVER << endl
             << "    Port:   " << PORT << endl;
    }
    
    return true;
}
//------------------------------------------------------
bool IsNum(string str) {
    for(int i = 0; i < static_cast<int>(str.length()); i++) {
        if(!isdigit(str.at(i)))
            return false;
    }
    return true;
}
//------------------------------------------------------
void DisplayBanner() {
    cout << endl
         << "          +-------------------------------------------+" << endl
         << "          |               AI for BZFLAG               |" << endl
         << "          +-------------------------------------------+" << endl
         << "                Nephi Johnson & William Armstrong" << endl
         << "                             CS470" << endl
         << endl;
}
//------------------------------------------------------
void DisplayUsage() {
    cout << "USAGE: " << arg0 << " [-d] <server> <port>" << endl
         << endl
         << "    -d       --    DEBUG MODE" << endl
         << "    server   --    bzrobots server " << endl
         << "    port     --    bzrobots port " << endl << endl << endl;
}
//------------------------------------------------------



