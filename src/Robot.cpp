

#include "Robot.h"
#include "RobotController.h"
#include "MyTank.h"

#include <cmath>
#include <math.h>
#include <vector>
#include <string.h>
#include <string>
#include <iostream>

using namespace std;


float PI = 4.0*atan(1.0);


/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */


//------------------------------------------------------
Robot::Robot(MyTank *meTank, BZFSCommunicator *bzfsComm, EnvironmentData *env) {
    this->meTank = meTank;
    this->bzfsComm = bzfsComm;
    this->env = env;
}
//------------------------------------------------------
void Robot::BeAlive() {
    sleep(2);
    cout << "I AM ALIVE!! (" << this->meTank->ToString() << ")" << endl;

    bzfsComm->speed(meTank->index, 0.7);
    bool turnedOnce = false;
    //cout << "here we are" << endl;
    //PrintGnuplotInfo();
    //bool doYourWork = true;
    while(1 == 1) {
        //cout << "tanks# -- " << meTank->index << endl;
        bzfsComm->shoot(meTank->index);

        float xForce = 0;
        float yForce = 0;
        float meX = meTank->pos[0];
        float meY = meTank->pos[1];
        bool hasFlag = (meTank->flag != "none");

        GenerateField(meX, meY, &xForce, &yForce, bzfsComm->myColor, hasFlag);

        float finalAngle = Wrap(atan2(yForce,xForce), PI*2);//good
        float angleDiff = GetAngleDist(meTank->angle, finalAngle);//not good
        float angVel = (angleDiff/(2*PI))*0.8;

        if(angVel < 0)
            angVel -= 0.2;
        else
            angVel += 0.2;

        float speed = 1 - abs(angleDiff/PI);
        //cout << "anglediff: " << (angleDiff/PI) << ", speed: " << speed << endl;
        bzfsComm->speed(meTank->index, speed);
        bzfsComm->angvel(meTank->index, angVel);

        turnedOnce = true;
        usleep(400);
    }
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
//------------------------------------------------------




/* +--------------------------------+
 * |           PRIVATE              |
 * +--------------------------------+  */



//------------------------------------------------------
void Robot::GenerateField(float x, float y, float *outX, float *outY, string color, bool haveFlag) {
    float xForce;
    float yForce;

    if(!haveFlag) {
        SetEnemyBaseField(&xForce, &yForce);
        SetEnemyField(&xForce, &yForce);
    } else {
        SetMyBaseField(&xForce, &yForce);
    }
    
    SetObstaclesField(&xForce, &yForce);

    *outX = xForce;
    *outY = yForce;
}
//------------------------------------------------------
void Robot::SetEnemyField(float *forceX, float *forceY) {
    
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
void Robot::UpdatePosition() {
    
}
//------------------------------------------------------








