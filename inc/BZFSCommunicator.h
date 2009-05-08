#ifndef BZFSCommunicator_CLASS
#define BZFSCommunicator_CLASS

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
        void Connect(string server, int port);
        string SendMessage(string message);


    private:
        void Disconnect();
        void ConnectToBZFS();
        string GetIPFromDomain(string server);
        bool IsIPAddress(string server);
        void ResolveDomain(string server);
};




#endif

