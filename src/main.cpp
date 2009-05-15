#include "RobotController.h"
#include "MyTank.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <cmath>
#include <math.h>

#include "GnuplotWriter.h"
#include "Node.h"


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
float GenerateField(float x, float y, float *outX, float *outY, string color, bool HAVE_FLAG);
bool HitObstacle(MyTank *tank);
float Wrap(float original, float max);
float GenerateField(float x, float y);
float GetCenterX(vector<Point> pts, int start);
float GetCenterY(vector<Point> pts, int start);
void PrintGnuplotInfo();
void SetCenterXY(vector<Point> pts, float *centerX, float *centerY);
void GnuplotTest();

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
    GnuplotTest();

    pthread_t threadRob1, threadRob2;
    int threadResultRob;
    int robNum = 0;
    cout << "Begin Robot Thread" << endl;
    pthread_create(&threadRob1, NULL, SmartRobot, (void*)&robNum);
    //pthread_create(&threadRob1, NULL, DummyRobot, (void*)&robNum);
    int robNum2 = 1;
    pthread_create(&threadRob2, NULL, SmartRobot, (void*)&robNum2);

    controller->LoopAction();
} 


void GnuplotTest() {
	GnuplotWriter* gw = new GnuplotWriter(&controller->env);
	Node startNode(-400, -400, 20);
	Node nextNode(-400, -380, 20);
	Node lastNode(-380,-360, 20);
	string s = gw->PrintNode(startNode, 2);
	s += gw->PrintAniData(2);
	s += gw->PrintNode(nextNode, 6);
	s += gw->PrintAniData(2);
	s += gw->PrintLine(startNode, nextNode, -1);
	s += gw->PrintAniData(2);
	s += gw->PrintNode(lastNode, 1);
	s += gw->PrintAniData(2);
	s += gw->PrintLine(nextNode, lastNode, -1);
	s += gw->PrintAniData(2);
	gw->PrintState(s, "GnuPlottest.gpi");

}

//------------------------------------------------------
void *SmartRobot(void *ptr ) {
    sleep(2);//let it get set up

	int *index = (int *) ptr;
    int tankIndex = *index;
    cout << "TANK INDEX: " << tankIndex << endl;

    MyTank *curTank = &controller->env.myTanks.at(tankIndex);
  
    controller->speed(tankIndex, 0.7);
    bool turnedOnce = false;

    PrintGnuplotInfo();
    bool doYourWork = true;
    while(doYourWork) {
        //calculate x and y potential field forces
        float xForce = 0;
        float yForce = 0;


       //set a random field value
        float randFloat = (rand()/RAND_MAX);

        float meX = curTank->pos[0];
        float meY = curTank->pos[1];

        bool hasFlag = curTank->flag != "none";

        GenerateField(meX, meY, &xForce, &yForce, "green", hasFlag);
        controller->shoot(tankIndex);

        float finalAngle = Wrap(atan2(yForce,xForce), PI*2);//good
        float angleDiff = GetAngleDist(curTank->angle, finalAngle);//not good
        float angVel = (angleDiff/(2*PI))*0.8;

        if(angVel < 0)
            angVel -= 0.2;
        else
            angVel += 0.2;

        //cout << "goal angle: " << finalAngle << "  currangle: " << curTank->angle << "  angleDiff: " << angleDiff << "  angVel: " << angVel << endl;
        //cout << " HAVE FLAG? " << (HAVE_FLAG ? "YES!!!" : "NO :(") << endl;

        float speed = 1 - abs(angleDiff/PI);
        cout << "anglediff: " << (angleDiff/PI) << ", speed: " << speed << endl;
        controller->speed(tankIndex, speed);

        /*
        if(abs(angleDiff) <= 0.05) 
            controller->speed(tankIndex, 1);
        else
            controller->speed(tankIndex, 0.2);
        */
        
            controller->angvel(tankIndex, angVel);


        //cout << "goal angle: " << me->angle << "   final angle: " << finalAngle << endl;
        //RotateDegrees(me, tankIndex, me->angle, angleDiff, angleDiff <= 0);
        turnedOnce = true;

        usleep(400);
    }
    /*
    */
}
void PrintGnuplotInfo() {
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
}
//------------------------------------------------------
float GenerateField(float x, float y, float *outX, float *outY, string color, bool HAVE_FLAG) {

    string myColor = "green";

    //vector<Obstacle> *obstacles = &controller->env.obstacles;
    
    EnvironmentData *env = &controller->env;

    vector<Flag *> enemyFlags;
    for(int i = 0; i < env->flags.size(); i++) {
        Flag *currFlag = &env->flags.at(i);
        //cout << "flag color: " << currFlag->color << endl;
        if(currFlag->color != color)
            enemyFlags.push_back(currFlag);
    }
    Base *myBase;
    vector<Base *> enemyBases;
    for(int i = 0; i < env->bases.size(); i++) {
        Base *currBase = &env->bases.at(i);
        if(currBase->color == color){
            myBase = currBase;
        } else {
            enemyBases.push_back(currBase);
        }
    }
    vector<Obstacle> obstacles = env->obstacles;

    float xForce;
    float yForce;

 
     if(!HAVE_FLAG) {
            //iterate enemyflags
        float FLAG_RADIUS = 5;
        float OBSTACLE_FACTOR = 5;
        float RAND_FACTOR = 2;

           for(int i = 0; i < enemyFlags.size(); i++) {
                Flag *currFlag = enemyFlags.at(i);
    
                float tempXForce = 0;
                float tempYForce = 0;
    
                float flagX = currFlag->pos[0];
                float flagY = currFlag->pos[1];
            
                SetPotentialFieldVals(&tempXForce, &tempYForce, x, x, flagX, flagY, true, FLAG_RADIUS, 200, 50);

                xForce += tempXForce;
                yForce += tempYForce;
            }

            //iterate enemyBases
            float BASE_RADIUS = 20;
            float BASE_SPREAD = 800;
            float BASE_ALPHA = 100;
            for(int i = 0; i < enemyBases.size(); i++) {
                Base *currBase = enemyBases.at(i);
                //cout << "ENEMY BASE COLOR: " << currBase->color << endl;  
    
                float tempXForce;
                float tempYForce;
    
                float baseX = currBase->corners.at(0).x;    
                float baseY = currBase->corners.at(0).y;
    		    float centerX = GetCenterX(currBase->corners, 0);
		        float centerY = GetCenterY(currBase->corners, 0);
	            
                SetPotentialFieldVals(&tempXForce, &tempYForce, x, y, centerX, centerY, true, BASE_RADIUS, BASE_SPREAD, BASE_ALPHA);    
                
                xForce += tempXForce;
                yForce += tempYForce;
                break;  //only go to the first base found
            }
        } else {
            //go home to your own base
            float BASE_RADIUS = 20;
            float BASE_SPREAD = 800;
            float BASE_ALPHA = 100;

            float tempXForce;
            float tempYForce;

 		    float centerX = GetCenterX(myBase->corners, 0);
		    float centerY = GetCenterY(myBase->corners, 0);
	
            //cout << "MYBASE CORNER X: " << myBaseX << "  MyBASE CORNER Y: " << myBaseY << endl;

            SetPotentialFieldVals(&tempXForce, &tempYForce, x, y, centerX, centerY, true, BASE_RADIUS, BASE_SPREAD, BASE_ALPHA);

            xForce += tempXForce;
            yForce += tempYForce;

            //cout << xForce << ", " << yForce << endl;
        }


         //iterate obstacles
         float OBST_RADIUS = .5;
         float OBST_SPREAD = 1;
         float OBST_ALPHA = -1000;
         for(int i = 0; i < obstacles.size(); i++) {
             Obstacle currObst = obstacles.at(i);
             //cout << "obstacle # corners: " << currObst.corners.size() << endl;
		float centerX;// = GetCenterX(currObst.corners, 0);
		float centerY;// = GetCenterY(currObst.corners, 0);
        SetCenterXY(currObst.corners, &centerX, &centerY);
		Point corner = currObst.corners.at(0);
		float radius = sqrt( (corner.x - centerX)*(corner.x - centerX) +
                       (corner.y - centerY)*(corner.y - centerY) );
		if (radius > 50) radius = 50;
        if (radius < 25) radius = 25;



/*
            for(int j = 0; j < currObst.corners.size(); j++) {
                Point corner = currObst.corners.at(j);
                float tempXForce;
                float tempYForce;
                
                SetPotentialFieldVals(&tempXForce, &tempYForce, x, y, corner.x, corner.y, false, radius, radius/2, OBST_ALPHA);

                if((!HAVE_FLAG && (j == 2 || j == 3)) || 
                    (HAVE_FLAG && (j == 0 || j == 1))) {
                    xForce -= tempYForce;
                    yForce += tempXForce;
                } else {
                    xForce += tempYForce;
                    yForce -= tempXForce;
                }
            }
*/

/*
		cout << "(" << currObst.corners.at(0).x << "," << currObst.corners.at(0).y << "),("
 				<< currObst.corners.at(1).x << "," << currObst.corners.at(1).y << "),("
				<< currObst.corners.at(2).x << "," << currObst.corners.at(2).y << "),("
				<< currObst.corners.at(3).x << "," << currObst.corners.at(3).y << "),("
				<< "center: (" << centerX << "," << centerY << ")" << " radius: " << radius << endl;*/

             //for(int j = 0; j < currObst.corners.size(); j++) {
               //  Point corner = currObst.corners.at(j);

 //ORIGINAL:::::::::::

                 float tempXForce;
                 float tempYForce;

                 SetPotentialFieldVals(&tempXForce, &tempYForce, x, y, centerX, centerY, false, radius+10/**OBST_RADIUS*/, radius/2 /** OBST_SPREAD*/, OBST_ALPHA);

                 //cout << " OBST X FORCE: " << tempXForce << "  OBST Y FORCE: " << tempYForce << endl;
                 /* ORIGINAL: (non-tangential)
                 xForce += tempXForce;
                 yForce += tempYForce;
                 */

                 xForce -= tempYForce;
                 yForce += tempXForce;
                 //
//END ORIGINAL::::::::::::::::::::
             //}
         }
         /*
         */
    
    *outX = xForce;
    *outY = yForce;
}
void SetCenterXY(vector<Point> pts, float *centerX, float *centerY) {
    if(pts.size() < 1)
        throw "Points must have more than one point!";
    float tempX = 0;
    float tempY = 0;
    for(int i = 0; i < pts.size(); i++) {
        Point currPoint = pts.at(i);
        tempX += currPoint.x;
        tempY += currPoint.y;
    }
    tempX /= pts.size();
    tempY /= pts.size();
    *centerX = tempX;
    *centerY = tempY;
}
float GetCenterX(vector<Point> pts, int start) {
	if (start >= pts.size())
		throw "Error finding center of obstacle";	
	float x1 = pts.at(start).x;
	float x2;
	if (start == pts.size() - 1)
		x2 = pts.at(0).x;
	else
		x2 = pts.at(start+1).x;
	if (x1 < x2) 
		return (x1 + abs((x2 - x1) / 2));
	else if(x2 < x1)
		return (x2 + abs((x1 - x2) / 2));
	else
		return GetCenterX(pts, start + 1);
}
float GetCenterY(vector<Point> pts, int start) {
	if (start >= pts.size())
		throw "Error finding center of obstacle";	
	float y1 = pts.at(start).y;
	float y2;
	if (start == pts.size() - 1)
		y2 = pts.at(0).y;
	else
		y2 = pts.at(start+1).y;
	if (y1 < y2) 
		return (y1 + abs((y2 - y1) / 2));
	else if(y2 < y1)
		return (y2 + abs((y1 - y2) / 2));
	else
		return GetCenterY(pts, start + 1);
}
//------------------------------------------------------
float GetAngleDist(float me, float goal) {

    float clockwise;
    float counterClockwise;
    float result = 0;

    if(me < goal) {
        clockwise = me + (2*PI - goal);
        counterClockwise = me - goal;
    } else if (me > goal) {
        clockwise = goal - me;
        counterClockwise = goal + (2*PI - me);
    }

    if(abs(clockwise) < abs(counterClockwise)) {
        result = clockwise;
        if(result > 0)
            result *= -1;
        cout << "clockwise!" << endl;;
    } else {
        result = abs(counterClockwise);
        cout << "             counterclockwise!" << endl;
    }

    return result;
}
//------------------------------------------------------
void SetPotentialFieldVals(float *xField, float *yField, float meX, float meY, float goalX, float goalY, bool attract, float radius, float spread, float alpha) {

    float dist = sqrt( (meX - goalX)*(meX - goalX) +
                       (meY - goalY)*(meY - goalY) );
    //cout << "  DIST: " << dist;
    float angle = atan2((goalY - meY),(goalX - meX));
    //cout << "  ANGLE: " << angle;

    //cout << endl;

    float deltaX;
    float deltaY;

    if(attract) {
        if(dist < radius) {
            deltaX = deltaY = 0;
        } else if (radius <= dist && dist <= radius + spread) {
            deltaX = alpha*(dist - radius)*cos(angle);
            deltaY = alpha*(dist - radius)*sin(angle);
        } else if (dist > spread + radius) {
            deltaX = alpha*spread*cos(angle);
            deltaY = alpha*spread*sin(angle);
        }
    } else {
        if(dist < radius) {
            deltaX = -1*cos(angle)*1000000000;
            deltaY = -1*sin(angle)*1000000000;
        } else if(radius <= dist && dist <= spread + radius) {
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



