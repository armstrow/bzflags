#ifndef BZFSCommunicator_CLASS
#define BZFSCommunicator_CLASS

#include "Robot.h"
#include <vector>
#include <string.h>
#include <string>
#include <iostream>

using namespace std;

class BZFSCommunicator {

 
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:
        string host;
        string ipaddress;
        int port;
        //ipaddress struct

    public:




    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    public:
        BZFSCommunicator();
        void Connect(string server, int port);
        string SendMessage(string message);


    private:
        void ConnectToBZFS();
};




#endif

