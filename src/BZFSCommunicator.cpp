

#include "BZFSCommunicator.h"

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


/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



//------------------------------------------------------
BZFSCommunicator::BZFSCommunicator() {

}
//------------------------------------------------------
void BZFSCommunicator::Connect(string server, int port) {
    this->host = server;
    this->port = port;

    ResolveDomain(server);
    ConnectToBZFS(); 
}
//------------------------------------------------------
string BZFSCommunicator::SendMessage(string message) {

}
//------------------------------------------------------






/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */


//------------------------------------------------------
void BZFSCommunicator::Disconnect() {
    shutdown(s, SHUT_RDWR);
}
//------------------------------------------------------
void BZFSCommunicator::ResolveDomain(string server) {
    //change server to an ip address if it's a domain
    if(!IsIPAddress(server))
    {
        server = GetIPFromDomain(server);

        //if debug?
        cout << "    domain (" << this->host << ") resolved to (" << server << ")" << endl;
    }
    ipAddress = server;
}
//------------------------------------------------------
void BZFSCommunicator::ConnectToBZFS() {
    cout << "    connecting to bzrobots at " << this->host << "[" << this->ipAddress << "]:" << this->port << "..." << endl;
    
    memset(&connection, 0, sizeof(connection));
    connection.sin_family = AF_INET;
    connection.sin_port = htons(port);
    
    if(!inet_aton(ipAddress.c_str(), &connection.sin_addr)) {
        cout << "inet conversion error\nCould not connect to server" << endl;
    }
    s = socket(PF_INET, SOCK_STREAM, 0);

    if(!s) {
        cout << "socket creation error" << endl;
    }

    if(connect(s, (const struct sockaddr *)&connection, sizeof(connection)) < 0) {
        cout << "socket connection error" << endl;
    }

    cout << "    connected!" << endl;
}
//------------------------------------------------------
bool BZFSCommunicator::IsIPAddress(string server) {
    for(int i = 0; i < static_cast<int>(server.length()); i++) {
        if(!isdigit(server.at(i)) && server.at(i) != '.')
            return false;
    }
    return true;
}
//------------------------------------------------------
string BZFSCommunicator::GetIPFromDomain(string domain) {
    string result;

    struct hostent *hostEntry;
    hostEntry = gethostbyname(domain.c_str());
    if(hostEntry == NULL)
    {
        result = "";
        return result;
    }
    
    if(hostEntry->h_addr_list[0] != NULL)
    {
        result = string(inet_ntoa(*(struct in_addr *)hostEntry->h_addr_list[0]));
    }
    else
    {
        result = "";
    }

    return result;   
}
//------------------------------------------------------



