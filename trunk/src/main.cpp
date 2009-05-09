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
float PI = 4.0*atan(1.0);

void *DummyRobot(void *ptr);
void Rotate60Degrees(MyTank *tank, int index, float originalAngle);
bool HitObstacle(MyTank *tank);
float Wrap(float original, float max);

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
    pthread_t threadRob1, threadRob2;
    int threadResultRob;
    int robNum = 0;
    cout << "Begin Robot Thread" << endl;
    pthread_create(&threadRob1, NULL, DummyRobot, (void*)&robNum);
    robNum = 1;
    pthread_create(&threadRob2, NULL, DummyRobot, (void*)&robNum);

    controller->LoopAction();
} 
//------------------------------------------------------
  
void *DummyRobot(void *ptr )
{
	usleep(500); //Let the MyTanks command run first thing to establish number of tanks

	int *index;
	index = (int *) ptr;
	while (1) {
	   if (controller->speed(*index, 0.5))
			cout << "tank moving!!!" << endl;
		else
			cout << "Couldn't Move Tank!" << endl;

		MyTank *curTank = &controller->env.myTanks.at(*index);
        vector<Obstacle> *obstacles = &controller->env.obstacles;
       
        int sleepAmount = 200;
        while(1 == 1) {
            if(curTank->status == "dead") { }
            else if(HitObstacle(curTank)) {
                cout << "hit obstacle" << endl;
                Rotate60Degrees(curTank, *index, curTank->angle);
            }
            if(curTank->angle != 0)
                controller->angvel(*index, 0);

            usleep(sleepAmount);
            controller->shoot(*index);
        }
	}
	
}
//------------------------------------------------------
bool HitObstacle(MyTank *tank) {
    cout << tank->ToString() << endl;
    if(abs(tank->velocity[0]) < 0.09 || abs(tank->velocity[1]) < 0.09)
        return true;
    return false;
}
//------------------------------------------------------
void Rotate60Degrees(MyTank *tank, int index, float originalAngle) {
    float goalAngle = Wrap((PI/3 + originalAngle), PI);

    controller->angvel(index, 1);
    sleep(2);
    controller->angvel(index, 0);
    controller->shoot(index);
}
//------------------------------------------------------
float Wrap(float original, float max) {
    while(original > max) {
        original -= max;
    }
    while(original < -1*max) {
        original += max;
    }
    return original;
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



