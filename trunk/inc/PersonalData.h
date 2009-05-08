#ifndef PERSONALDATA
#define PERSONALDATA


#include <iostream>

using namespace std;


class PersonalData {
 
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    public:
        double xpos;
        double ypox;
        double angvel;
        double speed;

    private:

 

    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    public:
        PersonalData();
        void Parse(string data);

    private:
 
};






#endif
