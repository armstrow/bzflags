#ifndef ROBOT
#define ROBOT


#include "PersonalData.h"
#include "BZFSCommunicator.h"

class Robot {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    public:

    private:
        PersonalData personalData;
        BZFSCommunicator *bzfsComm;



    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    public:
        Robot(BZFSCommunicator *bzfsComm);
        void GuardBase(double aggression);
        void AttackOthers(double aggression);
        void GetFlag(double agression);
        void SetPersonalData(string data);

    private:
        void UpdatePosition();
};




#endif
