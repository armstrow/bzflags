#ifndef ROBOT
#define ROBOT


#include "MyTank.h"
#include "BZFSCommunicator.h"
#include "EnvironmentData.h"
#include <vector>

class Robot {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    public:

    private:
        BZFSCommunicator *bzfsComm;
        MyTank *meTank;
        EnvironmentData *env;



    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    public:
        Robot(MyTank *meTank, BZFSCommunicator *bzfsComm, EnvironmentData *env);
        void GuardBase(double aggression);
        void AttackOthers(double aggression);
        void GetFlag(double agression);
        void SetPersonalData(string data);
        void BeAlive();

    private:
        void UpdatePosition();
        float Wrap(float original, float max);
        void GenerateField(float x, float y, float *outX, float *outY, string color, bool HAVE_FLAG);
        float GetAngleDist(float me, float goal);

        void SetCenterXY(vector<Point> pts, float *centerX, float *centerY);

        void SetEnemyBaseField(float *forceX, float *forceY);
        void SetEnemyField(float *forceX, float *forceY);
        void SetEnemyFlagField(float *forceX, float *forceY);
        void SetMyBaseField(float *forceX, float *forceY);
        void SetObstaclesField(float *forceX, float *forceY);

        void SetPotentialFieldVals(float *xField, float *yfield, float meX, float meY, float goalX, float goalY, bool attract, float radius, float spread, float alpha);
};




#endif
