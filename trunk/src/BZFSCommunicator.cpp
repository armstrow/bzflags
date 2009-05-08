

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

#define BUFFER_SIZE 1024

using namespace std;

char replyBuffer[BUFFER_SIZE];
int lineFeedPos = 0;
int start;

/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */



//------------------------------------------------------
BZFSCommunicator::BZFSCommunicator() {

}
//------------------------------------------------------
int BZFSCommunicator::Connect(string server, int port) {
    this->host = server;
    this->port = port;

    ResolveDomain(server);
    ConnectToBZFS(); 
    int shook = HandShake();
    if (shook == 1)
	cout << "Unable to shake hands with bzrobots.\n";
    else
	cout << "Handshake successfull!\n";
    return shook;
}
//------------------------------------------------------
vector <string> BZFSCommunicator::SendMessage(string message) {
	SendLine(message.c_str());
	char Reply[BUFFER_SIZE];
	ReadAck();
	return ReadArr();
}
//------------------------------------------------------






/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */
// Read line into vector
vector <string> BZFSCommunicator::ReadArr() {
	char str[BUFFER_SIZE];
	char *LineText=str;
	ReadLine(LineText);
	if(strlen(LineText)!=0) {
//		if(debug) 
		cout << LineText << endl;
	}
	while(strlen(LineText)==0) {
		ReadLine(LineText);
//		if(debug) 
		cout << LineText << endl;
	}
	return SplitString(LineText);
}
// Read Acknowledgement
void BZFSCommunicator::ReadAck() {
	vector <string> v=ReadArr();
	if(v.at(0)!="ack") {
		cout << "Did not receive ack! Exit!" << endl;
		exit(1);
	}
}

vector <string> BZFSCommunicator::SplitString(string str) {
	vector <string> MyVector;
	MyVector.begin();
	string MyString = str;
	MyVector.clear();
	size_t LastLoc = -1;
	size_t CurLoc = MyString.find(" ", 0);
	while (CurLoc != string::npos) {
		MyVector.push_back(MyString.substr(LastLoc+1, CurLoc-LastLoc-1));
		LastLoc=CurLoc;
		CurLoc = MyString.find(" ", LastLoc+1);
	}
	MyVector.push_back(MyString.substr(LastLoc+1, MyString.size()-LastLoc));
	return MyVector;
}

// Send line to server
int BZFSCommunicator::SendLine(const char *LineText) {
	int Length=(int)strlen(LineText);
	char Command[BUFFER_SIZE];
	strcpy(Command, LineText);
	Command[Length]='\n';
	Command[Length+1]='\0';
	//if(debug) 
	cout << Command;
	if (send(s, Command, Length+1, 0) >= 0) {
		return 0;
	}
	else {
		return 1;
	}
}

// Read line back from server
int BZFSCommunicator::ReadReply(char *Reply)
{
	char acReadBuffer[BUFFER_SIZE];
	int nNewBytes = recv(s, acReadBuffer, BUFFER_SIZE, 0);
	if (nNewBytes < 0) {
		return -1;
	}
	else if (nNewBytes == 0) {
		cerr << "Connection closed by peer." << endl;
		return 0;
	}
	
	memcpy(Reply, &acReadBuffer, nNewBytes);
	if(nNewBytes!=BUFFER_SIZE) {
		Reply[nNewBytes]='\0';
	}

	return nNewBytes;
}

// Only read one line of text from ReplyBuffer
void BZFSCommunicator::ReadLine(char *LineText) {
	memset(LineText, '\0', BUFFER_SIZE);
	// Only read more from server when done with current ReplyBuffer
	if(strlen(replyBuffer)==0) {
		char *Reply;
		Reply = replyBuffer;
		ReadReply(Reply);
	}
	int i=0;
	bool done=false;
	while(!done) {
		for(i=lineFeedPos+1; (i<BUFFER_SIZE && replyBuffer[i]); i++) {
			if(replyBuffer[i]=='\n') {
				LineText[i-lineFeedPos-1+start]=replyBuffer[i-1];
				LineText[i-lineFeedPos+start]='\0';
				lineFeedPos=i;
				start=0;
				done=true;
				break;
			}
			LineText[i-lineFeedPos-1+start]=replyBuffer[i-1];
		}
		if(!done) {
			start = (int)strlen(LineText);
			ResetReplyBuffer();	
			char *Reply;
			Reply = replyBuffer;
			ReadReply(Reply);
		}
		else {
			if(replyBuffer[i]=='\0') {
				done=true;
				start=0;
				ResetReplyBuffer();
			}
		}
	}
	cout << "Line Read: " << LineText;
}

// Reset the ReplyBuffer
void BZFSCommunicator::ResetReplyBuffer() {
	memset(replyBuffer, '\0', BUFFER_SIZE);
	lineFeedPos=0;
}

// Perform HandShake with the server
int BZFSCommunicator::HandShake() {
	char str[BUFFER_SIZE];
	char *LineText;
	LineText=str;
	ReadLine(LineText);
	//if(debug) 
	cout << LineText << endl;
	if (!strcmp(LineText, "bzrobots 1")) {
		const char * Command="agent 1";
		int temp=SendLine(Command);
		if(temp==1) 
			return 1;
		else
			ResetReplyBuffer();
			return 0;
	}
	else
		return 1;
}


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



