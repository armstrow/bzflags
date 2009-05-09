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
float GetAngleDist(float me, float goal);
void SetPotentialFieldVals(float *xField, float *yfield, float meX, float meY, float goalX, float goalY, bool attract, float radius, float spread, float alpha);
void RotateDegrees(MyTank *tank, int index, float originalAngle, float amount, bool right);
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
    pthread_create(&threadRob2, NULL, SmartRobot, (void*)&robNum2);

    controller->LoopAction();
} 
//------------------------------------------------------
void *SmartRobot(void *ptr ) {
    sleep(2);//let it get set up

	int *index = (int *) ptr;
    int tankIndex = *index;
    cout << "TANK INDEX: " << tankIndex << endl;

    string myColor = "green";

    MyTank *curTank = &controller->env.myTanks.at(tankIndex);
    //vector<Obstacle> *obstacles = &controller->env.obstacles;
    
    EnvironmentData *env = &controller->env;
    cout << "MY TANKS SIZE: " << env->myTanks.size() << endl;
    for(int i =0; i < env->myTanks.size(); i++) {
        cout << env->myTanks.at(i).ToString() << endl;
    }
    MyTank *me = &env->myTanks.at(tankIndex);
    me->color = myColor;
    vector<Flag *> enemyFlags;
    for(int i = 0; i < env->flags.size(); i++) {
        Flag *currFlag = &env->flags.at(i);
        cout << "flag color: " << currFlag->color << endl;
        if(currFlag->color != me->color)
            enemyFlags.push_back(currFlag);
    }
    Base *myBase;
    vector<Base *> enemyBases;
    for(int i = 0; i < env->bases.size(); i++) {
        Base *currBase = &env->bases.at(i);
        if(currBase->color == me->color){
            myBase = currBase;
        } else {
            enemyBases.push_back(currBase);
        }
    }
    vector<Obstacle> obstacles = env->obstacles;
  
    controller->speed(tankIndex, 0.5);
    bool turnedOnce = false;
    while(1 == 1) {
        //calculate x and y potential field forces
        float xForce = 0;
        float yForce = 0;

        bool HAVE_FLAG = me->flag != "none";

        float FLAG_RADIUS = 5;
        float OBSTACLE_FACTOR = 5;
        float RAND_FACTOR = 2;

        //set a random field value
        float randFloat = (rand()/RAND_MAX);

        float meX = me->pos[0];
        float meY = me->pos[1];

        if(!HAVE_FLAG) {
        //iterate enemyflags
        for(int i = 0; i < enemyFlags.size(); i++) {
            Flag *currFlag = enemyFlags.at(i);

            float tempXForce = 0;
            float tempYForce = 0;

            float flagX = currFlag->pos[0];
            float flagY = currFlag->pos[1];
        
            SetPotentialFieldVals(&tempXForce, &tempYForce, meX, meY, flagX, flagY, true, FLAG_RADIUS, 200, 50);

            xForce += tempXForce;
            yForce += tempYForce;
        }

        //iterate enemyBases
        float BASE_RADIUS = 20;
        float BASE_SPREAD = 500;
        float BASE_ALPHA = 100;
        for(int i = 0; i < enemyBases.size(); i++) {
            Base *currBase = enemyBases.at(i);
            //cout << "ENEMY BASE COLOR: " << currBase->color << endl;

            float tempXForce;
            float tempYForce;

            float baseX = currBase->corners.at(0).x;
            float baseY = currBase->corners.at(0).y;
            
            SetPotentialFieldVals(&tempXForce, &tempYForce, meX, meY, baseX, baseY, true, BASE_RADIUS, BASE_SPREAD, BASE_ALPHA);
            
            xForce += tempXForce;
            yForce += tempYForce;
            break;
        }
        } else {
            //iterate enemyBases
            float BASE_RADIUS = 20;
            float BASE_SPREAD = 500;
            float BASE_ALPHA = 100;

            float tempXForce;
            float tempYForce;

            SetPotentialFieldVals(&tempXForce, &tempYForce, meX, meY, myBase->corners.at(0).x, myBase->corners.at(0).y, true, BASE_RADIUS, BASE_SPREAD, BASE_ALPHA);
            xForce += tempXForce;
            yForce += tempYForce;
        }

        //iterate obstacles
        float OBST_RADIUS = 10;
        float OBST_SPREAD = 50;
        float OBST_ALPHA = -100;
        for(int i = 0; i < obstacles.size(); i++) {
            Obstacle currObst = obstacles.at(i);

            for(int j = 0; j < currObst.corners.size(); j++) {
                Point corner = currObst.corners.at(j);

                float tempXForce;
                float tempYForce;

                SetPotentialFieldVals(&tempXForce, &tempYForce, meX, meY, corner.x, corner.y, false, OBST_RADIUS, OBST_SPREAD, OBST_ALPHA);

                cout << " OBST X FORCE: " << tempXForce << "  OBST Y FORCE: " << tempYForce << endl;
                xForce += tempXForce;
                yForce += tempYForce;
            }
        }


        float finalAngle = Wrap(atan2(yForce,xForce), PI*2);
        float angleDiff = GetAngleDist(me->angle, finalAngle);
        float angVel = (angleDiff);

        cout << "goal angle: " << finalAngle << "  currangle: " << me->angle << "  angleDiff: " << angleDiff << "  angVel: " << angVel << endl;
        cout << " HAVE FLAG? " << (HAVE_FLAG ? "YES!!!" : "NO :(") << endl;

        if(angleDiff <= 0.05)
            controller->angvel(tankIndex, 0);
        else
            controller->angvel(tankIndex, angVel);

        //cout << "goal angle: " << me->angle << "   final angle: " << finalAngle << endl;
        //RotateDegrees(me, tankIndex, me->angle, angleDiff, angleDiff <= 0);
        turnedOnce = true;

        usleep(400);
    }
}
//------------------------------------------------------
float GetAngleDist(float me, float goal) {

    float clockwise;
    float counterClockwise;
    float result = 0;

    if(me < goal) {
        clockwise = me - goal;
        counterClockwise = (2*PI - me) + goal;
    } else if (me > goal) {
        clockwise = goal - me;
        counterClockwise = me + (2*PI - goal);
    }

    if(clockwise < counterClockwise)
        result = -1 * clockwise;
    else
        result = counterClockwise;

    return result;
}
//------------------------------------------------------
void SetPotentialFieldVals(float *xField, float *yField, float meX, float meY, float goalX, float goalY, bool attract, float radius, float spread, float alpha) {

    float dist = sqrt( (meX - goalX)*(meX - goalX) +
                       (meY - goalY)*(meY - goalY) );
    //cout << "  DIST: " << dist;
    float angle = atan2((goalY - meY),(goalX - meX));
    //cout << "  ANGLE: " << angle;

    cout << endl;

    float deltaX;
    float deltaY;

    if(attract) {
        if(dist < radius) {
            deltaX = deltaY = 0;
        } else if (radius <= dist <= radius + spread) {
            deltaX = alpha*(dist - radius)*cos(angle);
            deltaY = alpha*(dist - radius)*sin(angle);
        } else if (dist > spread + radius) {
            deltaX = alpha*spread*cos(angle);
            deltaY = alpha*spread*sin(angle);
        }
    } else {
        if(dist < radius) {
            deltaX = deltaY = -100;
        } else if(radius <= dist <= spread + radius) {
            deltaX = alpha*(spread + radius - dist)*cos(angle);
            deltaY = alpha*(spread + radius - dist)*sin(angle);
        } else if(dist > spread + radius) {
            deltaX = deltaY = 0;
        }
    }
    *xField = deltaX;
    *yField = deltaY;

    //cout << "       xfield: " << *xField << "   yfield: " << *yField << endl;
}
//------------------------------------------------------
void *DummyRobot(void *ptr )
{
	sleep(3); //Let the MyTanks command run first thing to establish number of tanks


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
}
//------------------------------------------------------
bool HitObstacle(MyTank *tank) {
    //cout << tank->ToString() << endl;
    if(abs(tank->velocity[0]) < 0.09 || abs(tank->velocity[1]) < 0.09)
        return true;
    return false;
}
//------------------------------------------------------
void RotateDegrees(MyTank *tank, int index, float originalAngle, float amount, bool right) {
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
}
//------------------------------------------------------
float Wrap(float original, float max) {
    while(original > max) {
        original -= max;
    }
    /*
    while(original < -1*max) {
        original += max;
    }
    */
    while(original < 0) {
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



