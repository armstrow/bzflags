#ifndef BZFSCommunicator_CLASS
#define BZFSCommunicator_CLASS


#include "Team.h"
#include "Obstacle.h"
#include "Shot.h"
#include "MyTank.h"
#include "OtherTank.h"
#include "Constant.h"
#include "Flag.h"
#include "Base.h"


#include <vector>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fstream>
#include <iostream>
#include <unistd.h>

using namespace std;

class BZFSCommunicator {
 
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */

    private:
        string host;
        string ipAddress;
        int port;
        struct sockaddr_in connection;
        int s;

    public:


    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */

    public:
        BZFSCommunicator();
        int Connect(string server, int port);
	/* Returns 0 for success, 1 for failure */
        vector <string> SendMessage(string message);//, bool resultIsList);
	    vector <string> ReadArr();
        bool get_teams(vector<Team> *allTeams);
        bool get_mytanks(vector<MyTank> *allMyTanks);
        bool get_othertanks(vector<OtherTank> *allOtherTanks);
        bool get_obstacles(vector<Obstacle> *allObstacles);
        bool get_flags(vector<Flag> *allFlags);
        bool get_shots(vector<Shot> *allShot);
        bool get_constants(vector<Constant> *allConstants);
        bool get_bases(vector<Base> *allBases);

    private:
	    void ReadAck();
    	vector <string> SplitString(string str);
    	int SendLine(const char* LineText);
    	int ReadReply(char *Reply);
    	void ReadLine(char *LineText);
    	void ResetReplyBuffer();
    	int HandShake();
        void Disconnect();
        int ConnectToBZFS();
        string GetIPFromDomain(string server);
        bool IsIPAddress(string server);
        void ResolveDomain(string server);
	
};






#endif

