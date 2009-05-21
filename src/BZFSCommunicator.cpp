

#include "BZFSCommunicator.h"
#include "Team.h"
#include "Obstacle.h"
#include "Shot.h"
#include "MyTank.h"
#include "OtherTank.h"
#include "Constant.h"
#include "Flag.h"

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
#include <pthread.h>


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
    if (ConnectToBZFS() < 0)
    	return 1; 
    cout << "shaking hands..." << endl;
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
	ReadAck();
	return ReadArr();
}

bool BZFSCommunicator::SendBoolMessage(string msg) {
	vector <string> reply = SendMessage(msg);
	if(reply.at(0) == "ok") {
		return true;
	}
	else {
		return false;
	}
}
//--------------------------------------------------------------------
bool BZFSCommunicator::shoot(int index) {
// Perform a shoot request.
	pthread_mutex_lock(&socket_lock);	    
	char char_buff[20];
	sprintf(char_buff, " %d", index);	
	string str_buff="shoot";
	str_buff.append(char_buff);
    bool result = SendBoolMessage(str_buff);
	pthread_mutex_unlock(&socket_lock);	    
	return result;
}
//--------------------------------------------------------------------
bool BZFSCommunicator::speed(int index, double value) {
// Set the desired speed to the specified value.
	pthread_mutex_lock(&socket_lock);	    
	char char_buff[20];
	sprintf(char_buff, " %d", index);	
	string str_buff="speed";
	str_buff.append(char_buff);
	sprintf(char_buff, " %f", value);
	str_buff.append(char_buff);
    bool result = SendBoolMessage(str_buff);
	pthread_mutex_unlock(&socket_lock);	    
	return result;
}

bool BZFSCommunicator::angvel(int index, double value) {
// Set the desired angular velocity to the specified value.
	pthread_mutex_lock(&socket_lock);	    
	char char_buff[20];
	sprintf(char_buff, " %d", index);	
	string str_buff="angvel";
	str_buff.append(char_buff);
	sprintf(char_buff, " %f", value);
	str_buff.append(char_buff);
    bool result = SendBoolMessage(str_buff);
	pthread_mutex_unlock(&socket_lock);	    
	return result;
}

bool BZFSCommunicator::accelx(int index, double value) {
// Set the desired accelaration in x axis to the specified value in hovertank mode.
	pthread_mutex_lock(&socket_lock);	    
	char char_buff[20];
	sprintf(char_buff, " %d", index);	
	string str_buff="accelx";
	str_buff.append(char_buff);
	sprintf(char_buff, " %f", value);
	str_buff.append(char_buff);
    bool result = SendBoolMessage(str_buff);
	pthread_mutex_unlock(&socket_lock);	    
	return result;
}

bool BZFSCommunicator::accely(int index, double value) {
// Set the desired accelaration in x axis to the specified value in hovertank mode.
	pthread_mutex_lock(&socket_lock);	    
	char char_buff[20];
	sprintf(char_buff, " %d", index);	
	string str_buff="accely";
	str_buff.append(char_buff);
	sprintf(char_buff, " %f", value);
	str_buff.append(char_buff);
    bool result = SendBoolMessage(str_buff);
	pthread_mutex_unlock(&socket_lock);	    
	return result;
}

//------------------------------------------------------
bool BZFSCommunicator::get_teams(vector<Team> *AllTeams) {
	pthread_mutex_lock(&socket_lock);	    
	SendLine("teams");
	ReadAck();
	vector <string> v = ReadArr();
    //cout << "teams v.at(0) = " << v.at(0) << endl;
	if(v.at(0)!="begin") {
	pthread_mutex_unlock(&socket_lock);	    
		return false;
	}
	v.clear();
	v=ReadArr();
	int i=0;
	while(v.at(0)=="team") {
		Team MyTeam(v);
        //cout << "team to string: " << MyTeam.ToString() << endl;
		AllTeams->push_back(MyTeam);
        //cout << "teams size: " << AllTeams->size() << endl;
		v.clear();
		v=ReadArr();
		i++;
	}
	if(v.at(0)!="end") {
	pthread_mutex_unlock(&socket_lock);	    
		return false;
	}
	pthread_mutex_unlock(&socket_lock);	    
	return true;
}
bool BZFSCommunicator::get_obstacles(vector<Obstacle> *AllObstacles) {
	pthread_mutex_lock(&socket_lock);	    
    cout << "locked it" << endl;
	// Request a list of obstacles.
    //cout << "    gettings obsts" << endl;
	SendLine("obstacles");
    cout << "sent line" << endl;
    //cout << "        sent line" << endl;
	ReadAck();
    cout << "read ack" << endl;
    //cout << "        read ack" << endl;
	vector <string> v=ReadArr();
	if(v.at(0)!="begin") {
	    pthread_mutex_unlock(&socket_lock);	    
        cout << "unlocked it 1" << endl;
    	return false;
	}
	v.clear();
	v=ReadArr();
	int i=0;
	while(v.at(0)=="obstacle") {
    	Obstacle MyObstacle(v);
	//cout << "reading obstacle: " << v.at(0);
    	AllObstacles->push_back(MyObstacle);
	    v.clear();
	    v=ReadArr();
	    i++;
        //cout << "        obst#: " << i << endl;
	}
	if(v.at(0)!="end") {
	    pthread_mutex_unlock(&socket_lock);	    
        cout << "unlocked it 2" << endl;
        //cout << "    --> get_obst: NO END" << endl;
    	return false;
	}
	pthread_mutex_unlock(&socket_lock);	    
        cout << "unlocked it 3" << endl;
   //cout << "    --> get_obst: DONE!" << endl;
   return true;
}
bool BZFSCommunicator::get_bases(vector<Base> *AllBases) {
	pthread_mutex_lock(&socket_lock);	    
	// Request a list of bases.
	SendLine("bases");
	ReadAck();
	vector <string> v=ReadArr();
	if(v.at(0)!="begin") {
	pthread_mutex_unlock(&socket_lock);	    
    	return false;
	}
	v.clear();
	v=ReadArr();
	int i=0;
	while(v.at(0)=="base") {
    	Base MyBase(v);
    	AllBases->push_back(MyBase);
	    v.clear();
	    v=ReadArr();
	    i++;
        //cout << "        obst#: " << i << endl;
	}
	if(v.at(0)!="end") {
	pthread_mutex_unlock(&socket_lock);	    
        //cout << "    --> get_obst: NO END" << endl;
    	return false;
	}
	pthread_mutex_unlock(&socket_lock);	    
   //cout << "    --> get_obst: DONE!" << endl;
   return true;
}
bool BZFSCommunicator::get_flags(vector<Flag> *AllFlags) {
	pthread_mutex_lock(&socket_lock);	    
    // Request a list of flags.
    SendLine("flags");
    ReadAck();
    vector <string> v=ReadArr();
    if(v.at(0)!="begin") {
	pthread_mutex_unlock(&socket_lock);	    
        return false;
    }
    v.clear();
    v=ReadArr();
    int i=0;
    while(v.at(0)=="flag") {
        int tIndex = atoi(v.at(1).c_str());
        if(AllFlags->size() < tIndex + 1) {
	        Flag MyFlag(v);
    	    AllFlags->push_back(MyFlag);
        } else {
            AllFlags->at(tIndex).SetData(v);
        }
    	v.clear();
    	v = ReadArr();
	    i++;
    }
    if(v.at(0)!="end") {
	pthread_mutex_unlock(&socket_lock);	    
        return false;
    }
	pthread_mutex_unlock(&socket_lock);	    
    return true;
}

bool BZFSCommunicator::get_shots(vector<Shot> *AllShots) {
	pthread_mutex_lock(&socket_lock);	    
    // Request a list of shots.
    SendLine("shots");
    ReadAck();
    vector <string> v=ReadArr();
    if(v.at(0)!="begin") {
	pthread_mutex_unlock(&socket_lock);	    
    return false;
    }
    v.clear();
    v=ReadArr();
    int i=0;
    while(v.at(0)=="shot") {
        int tIndex = atoi(v.at(1).c_str());
        if(AllShots->size() < tIndex + 1) {
	        Shot MyShot(v);
    	    AllShots->push_back(MyShot);
        } else {
            AllShots->at(tIndex).SetData(v);
        }
    	v.clear();
    	v = ReadArr();
	    i++;
    }
    if(v.at(0)!="end") {
	pthread_mutex_unlock(&socket_lock);	    
    return false;
    }
	pthread_mutex_unlock(&socket_lock);	    
    return true;
}

bool BZFSCommunicator::get_mytanks(vector<MyTank> *AllMyTanks) {
	pthread_mutex_lock(&socket_lock);	    
    // Request a list of our robots.
    SendLine("mytanks");
    ReadAck();
    vector <string> v=ReadArr();
    if(v.at(0)!="begin") {
	    pthread_mutex_unlock(&socket_lock);	    
    	return false;
    }
    v.clear();
    v=ReadArr();
    int i=0;
    while(v.at(0)=="mytank") {
        int tIndex = atoi(v.at(1).c_str());
        if(AllMyTanks->size() < tIndex + 1) {
	        MyTank t(v);
    	    AllMyTanks->push_back(t);
        } else {
            AllMyTanks->at(tIndex).SetData(v);
        }
    	v.clear();
    	v = ReadArr();
	    i++;
    }
    if(v.at(0)!="end") {
	pthread_mutex_unlock(&socket_lock);	    
    	//if(debug) 
	//cout << v.at(0) << endl;
    	return false;
    }
	pthread_mutex_unlock(&socket_lock);	    
    return true;
}

bool BZFSCommunicator::get_othertanks(vector <OtherTank> *AllOtherTanks) {
	pthread_mutex_lock(&socket_lock);	    
    // Request a list of tanks that aren't our robots.
    SendLine("othertanks");
    ReadAck();
    vector <string> v=ReadArr();
    if(v.at(0)!="begin") {
	    pthread_mutex_unlock(&socket_lock);	    
        return false;
    }
    v.clear();
    v=ReadArr();
    int i=0;
    while(v.at(0)=="othertank") {
        int tIndex = atoi(v.at(1).c_str());
        if(AllOtherTanks->size() < tIndex + 1) {
	        OtherTank TheOtherTank(v);
    	    AllOtherTanks->push_back(TheOtherTank);
        } else {
            AllOtherTanks->at(tIndex).SetData(v);
        }
    	v.clear();
    	v = ReadArr();
	    i++;
    }
    if(v.at(0)!="end") {
	pthread_mutex_unlock(&socket_lock);	    
        return false;
    }
	pthread_mutex_unlock(&socket_lock);	    
     return true;
}

bool BZFSCommunicator::get_constants(vector <Constant> *AllConstants) {
	pthread_mutex_lock(&socket_lock);	    
    // Request a dictionary of game constants.
    SendLine("constants");
    ReadAck();
    vector <string> v=ReadArr();
    if(v.at(0)!="begin") {
	    pthread_mutex_unlock(&socket_lock);	    
        return false;
    }
    v.clear();
    v=ReadArr();
    int i=0;
    while(v.at(0)=="constant") {
        Constant MyConstant(v);
        if(MyConstant.name == "team")
            this->myColor = MyConstant.value;
        else if(MyConstant.name == "worldsize")
            this->worldSize = atof(MyConstant.value.c_str());
	    AllConstants->push_back(MyConstant);
    	v.clear();
    	v=ReadArr();
    	i++;
    }
    if(v.at(0)!="end") {
	    pthread_mutex_unlock(&socket_lock);	    
    	return false;
    }
	pthread_mutex_unlock(&socket_lock);	    
    return true;
}






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
		//cout << LineText << endl;
	}
	while(strlen(LineText)==0) {
		ReadLine(LineText);
//		if(debug) 
		//cout << LineText << endl;
	}
	//cout << LineText << endl;
	return SplitString(LineText);
}
// Read Acknowledgement
void BZFSCommunicator::ReadAck() {
	vector <string> v=ReadArr();
	if(v.at(0)!="ack") {
		//cout << "Did not receive ack! Exit!" << endl;
		exit(1);
	}
}

vector <string> BZFSCommunicator::SplitString(string str) {
	vector <string> MyVector;
	MyVector.begin();
    int newLinePos = str.find("\n");
    while(newLinePos != -1)
    {
        str = str.erase(newLinePos, 1);
        newLinePos = str.find("\n");
    }
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
	//cout << Command;
    int amountSent = send(s, Command, Length+1, 0);
	if (amountSent >= 0) {
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
	//cout << "Line Read: " << LineText;
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
	//cout << LineText << endl;
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
int BZFSCommunicator::ConnectToBZFS() {
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

    if(int e = connect(s, (const struct sockaddr *)&connection, sizeof(connection)) < 0) {
        cout << "socket connection error" << endl;
        return e;
    }
	 
    cout << "    connected!" << endl;
    return 0;
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



