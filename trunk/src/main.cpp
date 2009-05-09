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
float PI = 4.0*atan(1.0);

void *DummyRobot(void *ptr);
void *SmartRobot(void *ptr);
void SetPotentialFieldVals(float *xField, float *yfield, float meX, float meY, float goalX, float goalY, float radius, float spread, float alpha);
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
    int robNum2 = 1;
    pthread_create(&threadRob2, NULL, DummyRobot, (void*)&robNum2);

    controller->LoopAction();
} 
//------------------------------------------------------
void *SmartRobot(int *ptr ) {
    usleep(500);//let it get set up

    int tankIndex = *ptr;

    string myColor = "green";

    EnvironmentData *env = &controller->env;
    MyTank *me = &env->myTanks.at(tankIndex);
    me->color = myColor;
    vector<Flag *> enemyFlags;
    for(int i = 0; i < env->flags.size(); i++) {
        Flag *currFlag = &env->flags.at(i);
        if(currFlag->color != me->color)
            enemyFlags.push_back(currFlag);
    }
    Base *myBase;
    for(int i = 0; i < env->bases.size(); i++) {
        Base *currBase = &env->bases.at(i);
        if(currBase->color == me->color){
            myBase = currBase;
        }
    }
    vector<Obstacle> obstacles = env->obstacles;
   
    while(1 == 1) {
        //calculate x and y potential field forces
        float xForce = 0;
        float yForce = 0;

        float FLAG_RADIUS = 100;
        float OBSTACLE_FACTOR = 5;
        float RAND_FACTOR = 2;

        //set a random field value
        float randFloat = (rand()/RAND_MAX);

        float meX = me->pos[0];
        float meY = me->pos[1];

        //iterate enemyflags
        for(int i = 0; i < enemyFlags.size(); i++) {
            Flag *currFlag = enemyFlags.at(i);

            float tempXForce = 0;
            float tempYForce = 0;

            float flagX = currFlag->pos[0];
            float flagY = currFlag->pos[1];
        
            SetPotentialFieldVals(&tempXForce, &tempYForce, meX, meY, flagX, flagY, FLAG_RADIUS, 10, 50);

            xForce += tempXForce;
            yForce += tempYForce;
        }

        //iterate obstacles


        float finalAngle = atan2(yForce,xForce);

        //move to the angle

        usleep(400);
    }
}
//------------------------------------------------------
void SetPotentialFieldVals(float *xField, float *yField, float meX, float meY, float goalX, float goalY, float radius, float spread, float alpha) {

    float dist = sqrt( (meX - goalX)*(meX - goalX) +
                       (meY - goalY)*(meY - goalY) );
    float angle = atan2((goalY - meY),(goalX - meX));

    float deltaX;
    float deltaY;

    if(dist < radius) {
        deltaX = deltaY = 0;
    } else if (radius <= dist <= radius + spread) {
        deltaX = alpha*(dist - radius)*cos(angle);
        deltaY = alpha*(dist - radius)*sin(angle);
    } else if (dist > spread + radius) {
        deltaX = alpha*spread*cos(angle);
        deltaY = alpha*spread*sin(angle);
    }
    *xField = deltaX;
    *yField = deltaY;
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
    float goalAngle = Wrap((originalAngle + PI/3 ), 2*PI);

    float error = abs(goalAngle - tank->angle);

    controller->angvel(index, 0.5);
    while(error > 0.1) {
        usleep(200);
        cout << "    --> " << tank->ToString() << endl;
        error = abs(goalAngle - tank->angle);
    }
    controller->angvel(index, 0);

    /*controller->angvel(index, 1);
    sleep(2);
    controller->angvel(index, 0);
    */
    //controller->shoot(index);
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



