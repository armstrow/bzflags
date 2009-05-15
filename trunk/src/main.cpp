#include "RobotController.h"
#include "MyTank.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <cmath>
#include <math.h>


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
void *SmartRobot(void *ptr);
float GetAngleDist(float me, float goal);
void SetPotentialFieldVals(float *xField, float *yfield, float meX, float meY, float goalX, float goalY, bool attract, float radius, float spread, float alpha);
void RotateDegrees(MyTank *tank, int index, float originalAngle, float amount, bool right);
float GenerateField(float x, float y, float *outX, float *outY, string color, bool HAVE_FLAG);
bool HitObstacle(MyTank *tank);
float Wrap(float original, float max);
float GenerateField(float x, float y);
float GetCenterX(vector<Point> pts, int start);
float GetCenterY(vector<Point> pts, int start);
void PrintGnuplotInfo();
void SetCenterXY(vector<Point> pts, float *centerX, float *centerY);
//void *MakeRobot(void *currTank);



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
    controller->PlayGame();
    cout << "DONE WITH GAME!!!" << endl;
}
//------------------------------------------------------
void PrintGnuplotInfo() {
    /*
    vector<Obstacle> obstacles = controller->env.obstacles;
    EnvironmentData *env = &controller->env;

    ofstream output;
    output.open("output.txt");

    output << "\n\nOBSTACLES" << endl << endl;
    output << "(";
    for(int i = 0; i < obstacles.size(); i++) {
        Obstacle currObst = obstacles.at(i);
        output << endl << "(";
        for(int j = 0; j < currObst.corners.size(); j++) {
            Point corner = currObst.corners.at(j);
            if(j != 0)
                output << ", ";
            output << "(" << corner.x << ", " << corner.y << ")";
        }
        output << ")";
        if(i != obstacles.size() -1)
            output << ", ";
    }
    output << ")" << endl;

    output << "\n\nFIELD VALUES" << endl << endl;

    output << "(";
    for(int x = -400; x <= 400; x += 12.5) {
        for(int y = -400; y <= 400; y+= 12.5) {
            float forceX;
            float forceY;
            GenerateField(x,y, &forceX, &forceY, "green", false);
            output << "((" << x << ", " << y << "), (" << forceX << ", " << forceY << "))";
            if(y != 400 || x != 400)
                output << ",\n";
        }
    }
    output << ")";

    output.close();
    */
}
//------------------------------------------------------
void *DummyRobot(void *ptr )
{
	sleep(3); //Let the MyTanks command run first thing to establish number of tanks

    /*
	int *index;
	index = (int *) ptr;
	while (1) {
	   if (controller->speed(*index, 0.5))
			cout << "tank moving!!!" << endl;
		else
			cout << "Couldn't Move Tank!" << endl;

        cout << "DUMMY MY TANKS SIZE: " << controller->env.myTanks.size() << endl;
		MyTank *curTank = &controller->env.myTanks.at(*index);
        vector<Obstacle> *obstacles = &controller->env.obstacles;
       
        int sleepAmount = 200;
        while(1 == 1) {
            if(curTank->status == "dead") { }
            else if(HitObstacle(curTank)) {
                cout << "hit obstacle" << endl;
                RotateDegrees(curTank, *index, curTank->angle, PI/3, true);
            }
            if(curTank->angle != 0)
                controller->angvel(*index, 0);

            usleep(sleepAmount);
            controller->shoot(*index);
        }
	}
    */
}
//------------------------------------------------------
bool HitObstacle(MyTank *tank) {
    //cout << tank->ToString() << endl;
    if(abs(tank->velocity[0]) < 0.09 || abs(tank->velocity[1]) < 0.09)
        return true;
    return false;
}
void RotateDegrees(MyTank *tank, int index, float originalAngle, float amount, bool right) {
    /*
    float goalAngle = Wrap((originalAngle + amount ), 2*PI);

    float error = abs(goalAngle - tank->angle);

    if(right)
        controller->angvel(index, -0.5);
    else
        controller->angvel(index, 0.5);
    while(error > 0.05) {
        usleep(200);
        //cout << "    --> " << tank->ToString() << endl;
        if(index == 1)
            cout << "goal angle: " << goalAngle << "   currAngle: " << tank->angle << endl;
        error = abs(goalAngle - tank->angle);
    }
    controller->angvel(index, 0);
    */
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
