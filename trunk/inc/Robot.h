#ifndef ROBOT
#define ROBOT


#include "MyTank.h"
#include "SearchAlg.h"
#include "AStarAlg.h"
#include "Node.h"
#include "BZFSCommunicator.h"
#include "EnvironmentData.h"
#include "KalmenFilter.h"

#include <vector>

#define SNIPER "sniper"
#define DECOY "decoy"
#define TRAVEL "travel"
#define MOVE_SNIPER "move_sniper"

#define CP_DUCK "clayPidgeonDuck"
#define CP_CONST_XY_VEL "clayPidgeonConstXYVel"
#define CP_CONST_XY_ACC "clayPidgeonConstXYAcc"
#define CP_GAUSS "clayPidgeonGauss"
#define CP_WILD "clayPidgeonWild"


class Robot {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    public:
        string currEnemyCallSign;
        bool gotoPoint;
        float gotoX;
        float gotoY;
        string actionType;
        vector<vector<Node *> > WorldNodes;
        Point currGoal;
        vector<Position> *currentPath;
        vector<Position> forwardsPath;
        vector<Position> backPath;
        float worldSize;
        BZFSCommunicator *bzfsComm;
        MyTank *meTank;
        float prevDecoyX;

    private:
	bool isDecoy;
        EnvironmentData *env;
        GnuplotWriter gpw;
        AStarAlg *alg;
		int WildCounter;
		KalmenFilter * kf;
		KalmenFilter * myKF;
		int kfCount;


    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    public:
        Robot(MyTank *meTank, BZFSCommunicator *bzfsComm, EnvironmentData *env, string robotStartType);
        void GuardBase(double aggression);
        void AttackOthers(double aggression);
        void GetFlag(double agression);
        void SetPersonalData(string data);
        void SwitchTo(string type);
        void Update();
	

    private: 
        string GetCurrEnemyStatus(string callSign);
        int GetCallSignIndex(string callSign);
        string GetClosestCallSign();
        float PDController(float goalAngle, float currAngle, float currAngVel);
        void SetNextPathNodeField(float *xForce, float *yForce);
        void UpdateCurrGoal();
        void SetCurrGoalToEnemyBase();
        void SetCurrGoalToMyBase();
        Position GetEndNode();
        Position GetStartNode();
        Position GetNode(float x, float y);
        bool IsVisitable(Node* n);
        void DiscretizeWorld();
		void SetMyFlagField(float *forceX, float *forceY);

        void DoTravel();
        void DoSniper();
        void DoMoveSniper();
        void DoDecoy();
        void DoCPDuck();
        void DoCPConstXYVel();
        void DoCPConstXYAcc();
        void DoCPGauss();
        void DoCPWild();

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
        void SetGotoField(float *forceX, float *forceY) ;

        void SetPotentialFieldVals(float *xField, float *yfield, float meX, float meY, float goalX, float goalY, bool attract, float radius, float spread, float alpha);
};




#endif
