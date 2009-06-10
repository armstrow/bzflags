

#include "Robot.h"
#include "RobotController.h"
#include "MyTank.h"
#include "AStarAlg.h"
#include "boxmuller.c"


#include <cmath>
#include <math.h>
#include <vector>
#include <string.h>
#include <string>
#include <iostream>

#define DECOY_DISTANCE 100
#define NODE_SIZE 10 
#define TARGET_COLOR "green"
#define SLEEP_AMT 100

#define NUM_OBSERVATIONS 0
#define PREDICTION_TIME 100

using namespace std;


float PI = 4.0*atan(1.0);

float sleepCount = 0;


/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */


//------------------------------------------------------
Robot::Robot(MyTank *meTank, BZFSCommunicator *bzfsComm, EnvironmentData *env): gpw(env) {
    this->meTank = meTank;
    this->bzfsComm = bzfsComm;
    this->env = env;
    this->gotoPoint = false;
    this->gotoX = 0;
    this->gotoY = 0;
    this->actionType = "BOGUSNESS";

    DiscretizeWorld();

    SetCurrGoalToEnemyBase();

    Position startNode = GetStartNode();
    Position endNode = GetEndNode();

    cout << "   endNode for A*: ";
    endNode.ToString();
    cout << endl;

    //float answer = box_muller(0.5, 0.5);

    alg = new AStarAlg(&WorldNodes, &gpw, true, env);
    alg->DoSearch(startNode, endNode, &forwardsPath);

    cout << "FINISHED INITIAL SEARCH!!! " << meTank->index << ",  path size: " << forwardsPath.size() << endl;

    for(int i = forwardsPath.size() - 1; i >= 0; i--) {
        backPath.push_back(forwardsPath.at(i));
    }

    if(backPath.size() > 4) {
        backPath.pop_back();
        backPath.pop_back();
        backPath.pop_back();
        backPath.pop_back();
    }

    //give the tank some room to get started with
    if(forwardsPath.size() > 4) {
        forwardsPath.pop_back();
        forwardsPath.pop_back();
        forwardsPath.pop_back();
        forwardsPath.pop_back();
    }

    currentPath = &forwardsPath;
	WildCounter = 0;
	kf = new KalmenFilter(env);
	myKF = new KalmenFilter(env);
	kfCount = 0;
}
//------------------------------------------------------
void Robot::DiscretizeWorld() {
    int NodeSize = NODE_SIZE;

    worldSize = bzfsComm->worldSize;
    //cout << "world size: " << worldSize << endl;
    if ((int)worldSize % (int)NodeSize != 0) {
        //cout << "NODE_SIZE does not divide evenly" << endl;
        exit(0);
    }
    worldSize = worldSize / 2;

    for (int x = 0 - worldSize; x < worldSize; x += NodeSize) {
        vector<Node *> tmp;
        for (int y = 0 - worldSize; y < worldSize; y += NodeSize) {
            Node *n = new Node(x, y, NodeSize);
            n->visitable = IsVisitable(n);
            tmp.push_back(n);
        }
        WorldNodes.push_back(tmp);
    }
    //cout << "Created WorldNodes size: " << WorldNodes.size() << endl;
}
//------------------------------------------------------
bool Robot::IsVisitable(Node* n) {
    //Get Center
    float testx = n->x + (n->length / 2);
    float testy = n->y + (n->length / 2);
    int nvert;
    bool c = false;

    for (int o = 0; o < env->getObstacles().size(); o ++) {
        Obstacle currObst = env->getObstacles().at(o);
        nvert = currObst.corners.size();
        float verty[4] = {currObst.corners.at(0).y, currObst.corners.at(1).y, currObst.corners.at(2).y, currObst.corners.at(3).y};
        float vertx[4] = {currObst.corners.at(0).x, currObst.corners.at(1).x, currObst.corners.at(2).x, currObst.corners.at(3).x};
        for (int i = 0, j = nvert-1; i < nvert; j = i++) {
            if ( ((verty[i]>testy) != (verty[j]>testy)) &&
                    (testx < (vertx[j]-vertx[i]) * (testy-verty[i]) / (verty[j]-verty[i]) + vertx[i]) )
                c = !c;
        }
        if (c)
            return !c;
    }
    return true;
}
//------------------------------------------------------
void Robot::Update() {
    //UpdateCurrGoal();

    if(this->actionType.compare(TRAVEL) == 0)
        DoTravel();
    else if(this->actionType.compare(DECOY) == 0)
        DoDecoy();
    else if(this->actionType.compare(SNIPER) == 0)
        DoSniper();
    else if(this->actionType.compare(CP_DUCK) == 0)
        DoCPDuck();
    else if(this->actionType.compare(CP_CONST_XY_VEL) == 0)
        DoCPConstXYVel();
    else if(this->actionType.compare(CP_CONST_XY_ACC) == 0)
        DoCPConstXYAcc();
    else if(this->actionType.compare(CP_GAUSS) == 0)
        DoCPGauss();
    else if(this->actionType.compare(CP_WILD) == 0)
        DoCPWild();
}
//------------------------------------------------------
float Robot::PDController(float goalAngle, float angleDiff, float currAngVel) {
    angleDiff /= PI;
    bool negDiff = angleDiff < 0;
    float newAngVel;
    if(abs(angleDiff) > 0.09)
        newAngVel = 1;
    else
        newAngVel = sqrt(abs(angleDiff));
    if(negDiff)
        newAngVel *= -1;

    float newSpeed;
    if(angleDiff < 0.3)
        newSpeed = 1;
    else
        newSpeed = 1 - sqrt(abs(angleDiff));

    //cout << "PDC  angleDiff: " << angleDiff << ", " << "newSpeed: " << newSpeed << ", newAngVel " << newAngVel << endl;

    bzfsComm->speed(meTank->index, newSpeed);
    bzfsComm->angvel(meTank->index, newAngVel);
}
//------------------------------------------------------
void Robot::DoCPDuck() {
    //be a sitting duck :)
    bzfsComm->speed(meTank->index, 0);
    bzfsComm->angvel(meTank->index, 0);
}
//------------------------------------------------------
void Robot::DoCPConstXYVel() {
    //keep the angvel at 0 and keep the speed at something
    bzfsComm->speed(meTank->index, 0.5);
    bzfsComm->angvel(meTank->index, 0);
}
//------------------------------------------------------
void Robot::DoCPConstXYAcc() {
    float currSpeed = sqrt(meTank->velocity[0]*meTank->velocity[0] + meTank->velocity[1]*meTank->velocity[1]);
    if(currSpeed < 1)
        bzfsComm->speed(meTank->index, currSpeed + 0.0001); //this will give us 10,000 cycles until he is up to speed
}
//------------------------------------------------------
void Robot::DoCPGauss() {
    
    float mean = 0;
    float stanDev = 1;
    float nextVal = box_muller(mean, stanDev);
    bzfsComm->speed(meTank->index, nextVal);
}
//------------------------------------------------------
void Robot::DoCPWild() {
	WildCounter++;
	if (WildCounter >= 20) WildCounter = 0;
	float offset = WildCounter / 10;
	bzfsComm->speed(meTank->index, ((-1-1)*((float)rand()/RAND_MAX))+1 + offset);
	sleep(1);//this will throw everything off, it's al in one thread now
	bzfsComm->angvel(meTank->index, ((-1-1)*((float)rand()/RAND_MAX))+1 + offset);
	sleep(1);
}
//------------------------------------------------------
void Robot::DoTravel() {
    gotoPoint = false;
    float xForce = 0;
    float yForce = 0;
    float meX = meTank->pos[0];
    float meY = meTank->pos[1];
    bool hasFlag = (meTank->flag != "none");
    if(hasFlag)
        SetCurrGoalToMyBase();

    GenerateField(meX, meY, &xForce, &yForce, bzfsComm->myColor, hasFlag);

    //cout << "DONE GENERating field" << endl;

    float finalAngle = Wrap(atan2(yForce,xForce), PI*2);//good
    float angleDiff = GetAngleDist(meTank->angle, finalAngle);//not good
    float currAngVel = meTank->angvel;
    PDController(finalAngle, angleDiff, currAngVel);
}
//------------------------------------------------------
float GetDistance(float startX, float startY, float endX, float endY) {
    float dist = sqrt( (startX - endX)*(startX - endX) +
            (startY - endY)*(startY - endY) );
    return dist;
}

void Robot::DoSniper() {
    gotoPoint = false;
    bzfsComm->speed(meTank->index, 0);

    float xDiff = 0;
    float yDiff = 0;
    float meX = meTank->pos[0];
    float meY = meTank->pos[1];
	float* myRslt = myKF->update(meX, meY);
	meX = myRslt[0];
	meY = myRslt[1];
	//TODO: Add Prediction of my position.

    float themX, themY;

    int i = 0;
    while (i < env->otherTanks.size() && env->otherTanks.at(i).status.compare("dead") == 0) i++; 
    if(env->otherTanks.size() == 0) {
        delete kf;
        kf = new KalmenFilter(env);
        kfCount = 0;
        cout << "`=`=`=`=``=`=`=`=`=`=`=`=`=`=`=`=`RESET" << endl;
        return;
    }
    if(i >= env->otherTanks.size()) {
        delete kf;
        kf = new KalmenFilter(env);
        kfCount = 0;
        cout << "`=`=`=`=``=`=`=`=`=`=`=`=`=`=`=`=`RESET" << endl;
        i = env->otherTanks.size() - 1;
        themX = env->otherTanks.at(i).x;
        themY = env->otherTanks.at(i).y;
        kf->setInitialTankPos(themX, themY);
    }
    themX = env->otherTanks.at(i).x;
    themY = env->otherTanks.at(i).y;

	float* rslt = kf->update(themX, themY);
	themX = rslt[0];
	themY = rslt[1];
    float result8;
	if (++kfCount > NUM_OBSERVATIONS) {
		kfCount = NUM_OBSERVATIONS+1;
        result8 = GetDistance(meX, meY, themX, themY)/PREDICTION_TIME;
        cout << "WHAT IS THIS NUMBER???? ::> " << result8 << endl;
		float* rslt = kf->predict(3+int(1.5*result8));//(3+int(3*result8));
		themX = rslt[0];
		themY = rslt[1];
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	xDiff = themX - meX;
	yDiff = themY - meY;
	float finalAngle = Wrap(atan2(yDiff,xDiff), PI*2);
	float angleDiff = GetAngleDist(meTank->angle, finalAngle);

    float accuracyRequirement = sqrt(0.04*(1/result8))/2.0;
    cout << "ACC REQ: " << accuracyRequirement << endl;
    if(kfCount > NUM_OBSERVATIONS && abs(angleDiff) < accuracyRequirement)
        bzfsComm->shoot(meTank->index);
	//float angVel = (angleDiff/(2*PI));

	angleDiff /= PI;
    bool negDiff = angleDiff < 0;
    float newAngVel = 1;

    if(abs(angleDiff) > 0.05)
        newAngVel = 1;
    else
        newAngVel = sqrt(abs(angleDiff));
    if(newAngVel < 0.05)
        newAngVel = 0.05;

    if(negDiff && newAngVel > 0)// {
        newAngVel *= -1;

   /*     newAngVel -= 0.1;
    } else {
        newAngVel += 0.2;
    }
    */

    cout << "NEW ANGVEL: " << newAngVel << endl;
	bzfsComm->angvel(meTank->index, newAngVel);
	//bzfsComm->shoot(meTank->index);
}
//------------------------------------------------------
void Robot::DoDecoy() {
    int speed = 1;

    //keep him perpindicular to the base
    float angleDiff = GetAngleDist(meTank->angle, PI*.5);
    float angVel = (angleDiff/(2*PI));
    if(abs(angleDiff) >= 0.1) {
        if(angleDiff < 0)
            bzfsComm->angvel(meTank->index, -0.7);
        else
            bzfsComm->angvel(meTank->index, 0.7);
    }

    float meY = meTank->pos[1];
    if(meY < -200)
        bzfsComm->speed(meTank->index, 1);
    else if(meY > 200)
        bzfsComm->speed(meTank->index, -1);

    /*
    */
}
//------------------------------------------------------
void Robot::SwitchTo(string type) {
    cout << "switching types from" << this->actionType << "to " << type << endl;
    actionType = type;
    cout << "actiontype is now: " << this->actionType << endl;
}
//------------------------------------------------------
void Robot::GuardBase(double aggression) {

}
//------------------------------------------------------
void Robot::AttackOthers(double aggression) {

}
//------------------------------------------------------
void Robot::GetFlag(double aggression) {

}

/* +--------------------------------+
 * |           PRIVATE              |
 * +--------------------------------+  */



//------------------------------------------------------
void Robot::GenerateField(float x, float y, float *outX, float *outY, string color, bool haveFlag) {
    float xForce;
    float yForce;

    SetNextPathNodeField(&xForce, &yForce);
    /*
    if(!haveFlag) {
        //SetEnemyBaseField(&xForce, &yForce);
        //SetEnemyField(&xForce, &yForce);
    } else {
        SetMyBaseField(&xForce, &yForce);
    }
    */

    //SetObstaclesField(&xForce, &yForce);

    *outX = xForce;
    *outY = yForce;
}
//------------------------------------------------------
void Robot::SetNextPathNodeField(float *forceX, float *forceY) {

    if(currentPath->size() == 0) {
        //done!
        bzfsComm->speed(meTank->index, 0);
        bzfsComm->angvel(meTank->index, 0);
        cout << "DONE FOLLOWING PATH!" << endl;
        return;
    }

    Position currentGoal = currentPath->back();

    float nodeSize = WorldNodes.at(0).at(0)->length;
    float currXGoal = (worldSize*-1 + currentGoal.row*nodeSize) + nodeSize/2;
    float currYGoal = (worldSize*-1 + currentGoal.col*nodeSize) + nodeSize/2;;
    float dist = (currXGoal - meTank->pos[0])*(currXGoal - meTank->pos[0]) + (currYGoal - meTank->pos[1])*(currYGoal - meTank->pos[1]);
 
    Position myPos = GetStartNode();
    if((dist < 1000 && currentPath->size() != 1) || (dist < 1 && currentPath->size() == 1)) {
        currentPath->pop_back();
    }
    Position nextPosition = currentPath->back();
    float xGoal = (worldSize*-1 + nextPosition.row*nodeSize) + nodeSize/2;
    float yGoal = (worldSize*-1 + nextPosition.col*nodeSize) + nodeSize/2;

    float RADIUS = 0;//they will never be there until they are there
    float SPREAD = NODE_SIZE*10;
    float ALPHA = 1000;

    float tempXForce = 0;
    float tempYForce = 0;

    SetPotentialFieldVals(&tempXForce, &tempYForce, meTank->pos[0], meTank->pos[1], xGoal, yGoal, true, RADIUS, SPREAD, ALPHA);

    *forceX += tempXForce;
    *forceY += tempYForce;
}
//------------------------------------------------------
Position Robot::GetEndNode() {
    return GetNode(currGoal.x, currGoal.y);
}
//------------------------------------------------------
Position Robot::GetStartNode() {
    return GetNode(meTank->pos[0], meTank->pos[1]);
}
//------------------------------------------------------
Position Robot::GetNode(float xloc, float yloc) {
    int ret[2];
    double length = WorldNodes.at(0).at(0)->length;
    for (int c = 0; c < WorldNodes.size(); c++) {
        if ((WorldNodes.at(0).at(c)->y - yloc) > (0 - length)) {
            ret[1] = c;
            break;
        }
    } for (int r = 0; r < WorldNodes.size(); r++) {
        if ((WorldNodes.at(r).at(0)->x - xloc) > (0 - length)) {
            ret[0] = r; break;
        }
    }
    Position p(ret[0], ret[1]);
    return p;
}
//------------------------------------------------------
void Robot::SetGotoField(float *forceX, float *forceY) {
    //iterate enemyflags
    float RADIUS = 5;
    float SPREAD = 100;
    float ALPHA = 1000;

    float tempXForce = 0;
    float tempYForce = 0;

    SetPotentialFieldVals(&tempXForce, &tempYForce, meTank->pos[0], meTank->pos[1], gotoX, gotoY, true, RADIUS, SPREAD, ALPHA);

    *forceX += tempXForce;
    *forceY += tempYForce;
}

//------------------------------------------------------
void Robot::SetEnemyField(float *forceX, float *forceY) {
    
}
//------------------------------------------------------
void Robot::UpdateCurrGoal() {
    bool hasFlag = (meTank->flag != "none");

    //cout << "ACTION TYPE: " << actionType << endl;
    //cout << "HAS FLAG:    " << (hasFlag ? "YES" : "NO") << endl;
    SetCurrGoalToEnemyBase();
    if(actionType == TRAVEL && hasFlag) {
        SetCurrGoalToMyBase();
    } else if(actionType == TRAVEL && !hasFlag) {
        SetCurrGoalToEnemyBase();
    }
}
//------------------------------------------------------
void Robot::SetCurrGoalToEnemyBase() {
    Base *selectedBase;
    for(int i = 0; i < env->bases.size(); i++) {
        if(env->bases.at(i).color == TARGET_COLOR) {
            selectedBase = &env->bases.at(i);
            break;
        }
    }
    
    float tempx;
    float tempy;

    for(int i = 0; i < selectedBase->corners.size(); i++) {
        Point currCorner = selectedBase->corners.at(i);
        tempx += currCorner.x;
        tempy += currCorner.y;
    }

    tempx *= 0.25;
    tempy *= 0.25;

    currGoal.x = tempx;
    currGoal.y = tempy;
}
//------------------------------------------------------
void Robot::SetCurrGoalToMyBase() {
    currentPath = &backPath;
    cout << "GOAL SET TO BACK PATH (setcurrgoaltomybase)" << endl;
}
//------------------------------------------------------
void Robot::SetEnemyBaseField(float *forceX, float *forceY) {
    float BASE_RADIUS = 20;
    float BASE_SPREAD = 800;
    float BASE_ALPHA = 100;
    for(int i = 0; i < env->bases.size(); i++) {
        Base *currBase = &env->bases.at(i);

        if(currBase->color == bzfsComm->myColor)
            continue;

        float tempXForce;
        float tempYForce;

        float baseX = currBase->corners.at(0).x;    
        float baseY = currBase->corners.at(0).y;
        float centerX, centerY;
        SetCenterXY(currBase->corners, &centerX, &centerY);

        SetPotentialFieldVals(&tempXForce, &tempYForce, meTank->pos[0], meTank->pos[1], centerX, centerY, true, BASE_RADIUS, BASE_SPREAD, BASE_ALPHA);    

        *forceX += tempXForce;
        *forceY += tempYForce;
        break;  //only go to the first base found
    }
}
//------------------------------------------------------
void Robot::SetEnemyFlagField(float *forceX, float *forceY) {
    //iterate enemyflags
    float FLAG_RADIUS = 5;
    float FLAG_SPREAD = 10;
    float FLAG_ALPHA = 100;

    for(int i = 0; i < env->flags.size(); i++) {
        Flag *currFlag = &env->flags.at(i);

        if(currFlag->color == bzfsComm->myColor)
            continue;

        float tempXForce = 0;
        float tempYForce = 0;

        float flagX = currFlag->pos[0];
        float flagY = currFlag->pos[1];

        SetPotentialFieldVals(&tempXForce, &tempYForce, meTank->pos[0], meTank->pos[1], flagX, flagY, true, FLAG_RADIUS, FLAG_SPREAD, FLAG_ALPHA);

        *forceX += tempXForce;
        *forceY += tempYForce;
    }
}
//------------------------------------------------------
void Robot::SetMyBaseField(float *forceX, float *forceY) {
    float BASE_RADIUS = 20;
    float BASE_SPREAD = 800;
    float BASE_ALPHA = 100;

    float tempXForce;
    float tempYForce;

    Base *myBase;
    for(int i = 0; i < env->bases.size(); i++) {
        Base *currBase = &env->bases.at(i);
        if(currBase->color == bzfsComm->myColor) {
            myBase = currBase;
            break;
        }
    }

    float centerX,centerY;
    SetCenterXY(myBase->corners, &centerX, &centerY);

    SetPotentialFieldVals(&tempXForce, &tempYForce, meTank->pos[0], meTank->pos[1], centerX, centerY, true, BASE_RADIUS, BASE_SPREAD, BASE_ALPHA);

    *forceX += tempXForce;
    *forceY += tempYForce;
}
//------------------------------------------------------
void Robot::SetObstaclesField(float *forceX, float *forceY) {
    //iterate obstacles
    float OBST_RADIUS = .5;
    float OBST_SPREAD = 1;
    float OBST_ALPHA = -1000;
    for(int i = 0; i < env->obstacles.size(); i++) {
        Obstacle currObst = env->obstacles.at(i);
        //cout << "obstacle # corners: " << currObst.corners.size() << endl;
        float centerX;// = GetCenterX(currObst.corners, 0);
        float centerY;// = GetCenterY(currObst.corners, 0);
        SetCenterXY(currObst.corners, &centerX, &centerY);
        Point corner = currObst.corners.at(0);
        float radius = sqrt( (corner.x - centerX)*(corner.x - centerX) +
                (corner.y - centerY)*(corner.y - centerY) );
        if (radius > 50) radius = 50;
        if (radius < 25) radius = 25;

        float tempXForce;
        float tempYForce;

        SetPotentialFieldVals(&tempXForce, &tempYForce, meTank->pos[0], meTank->pos[1], centerX, centerY, false, radius+10, radius/2, OBST_ALPHA);

        *forceX -= tempYForce;
        *forceY += tempXForce;
    }
}
//------------------------------------------------------
void Robot::SetPotentialFieldVals(float *xField, float *yField, float meX, float meY, float goalX, float goalY, bool attract, float radius, float spread, float alpha) {
    //cout << "meX:\t" << meX << ", meY:\t" << meY << ", goalX:\t" << goalX << ", goalY:\t" << goalY << endl;

    float dist = sqrt( (meX - goalX)*(meX - goalX) +
                       (meY - goalY)*(meY - goalY) );
    float angle = atan2((goalY - meY),(goalX - meX));

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
}
//------------------------------------------------------
void Robot::SetCenterXY(vector<Point> pts, float *centerX, float *centerY) {
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
//------------------------------------------------------
float Robot::GetAngleDist(float me, float goal) {
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
        //cout << "clockwise!" << endl;;
    } else {
        result = abs(counterClockwise);
        //cout << "             counterclockwise!" << endl;
    }

    return result;
}
//------------------------------------------------------
float Robot::Wrap(float original, float max) {
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








