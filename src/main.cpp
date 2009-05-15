#include "RobotController.h"
#include "MyTank.h"
#include "Flag.h"
#include "Base.h"
#include "Constant.h"
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
#include <pthread.h>
#include <cmath>
#include <math.h>
 
#include "GnuplotWriter.h"
#include "Node.h"
#include "SearchAlg.h"
#include "BreadthFirstAlg.h"


using namespace std;


void DisplayUsage();
void DisplayBanner();
bool ParseArgs(int argc, char** argv);
bool IsNum(string str);

char* arg0;
bool DEBUG = false;
string SERVER;
string STR_PORT;
int PORT;
RobotController* controller;
vector<vector<Node*> > *WorldNodes;

void *DummyRobot(void *ptr);
void *SmartRobot(void *ptr);
float GetAngleDist(float me, float goal);
void SetPotentialFieldVals(float *xField, float *yfield, float meX, float meY, float goalX, float goalY, bool attract, float radius, float spread, float alpha);
void RotateDegrees(MyTank *tank, int index, float originalAngle, float amount, bool right);
float GenerateField(float x, float y, float *outX, float *outY, string color, bool HAVE_FLAG);
bool HitObstacle(MyTank *tank);
float Wrap(float original, float max);
float GenerateField(float x, float y);
float GetCenterX(vector<Point> pts, int start);
float GetCenterY(vector<Point> pts, int start);
void PrintGnuplotInfo();
void SetCenterXY(vector<Point> pts, float *centerX, float *centerY);
//void *MakeRobot(void *currTank);
void GnuplotTest();
void DiscretizeWorld(double NodeSize);
Position GetStartNode();
Position GetEndNode();
Position GetNode(float xloc, float yloc);
bool IsVisitable(Node* n);
bool IsBelowLine(Point test, Point p1, Point p2);
bool IsAboveLine(Point test, Point p1, Point p2);
bool IsLeftOfLine(Point test, Point p1, Point p2);
bool IsRightOfLine(Point test, Point p1, Point p2);
float GetSlope(Point p1, Point p2);
float GetB(Point p, float slope);



//------------------------------------------------------
int main(int argc, char** argv) {

    DisplayBanner();

    arg0 = argv[0];

    if(argc < 3)
        DisplayUsage();
    else if(!ParseArgs(argc, argv)) {
        exit(0);
    }
    double NodeS = 10;
    if (DEBUG && argc == 5)
	NodeS = atof(argv[4]);
    else if (!DEBUG && argc == 4)
	NodeS = atof(argv[3]);

    controller = new RobotController(SERVER, PORT);
    DiscretizeWorld(NodeS);
    //GnuplotTest();

    /* Searches */
    cout << "Running search algorithms" << endl;
    Position startNode = GetStartNode();
    Position endNode = GetEndNode();
    string s = "";
    GnuplotWriter* gw = new GnuplotWriter(&controller->env);
    BreadthFirstAlg* bfs = new BreadthFirstAlg(WorldNodes, gw);
    s += bfs->DoSearch(startNode, endNode);

    //Remember to clear the "visited" boolean in the Nodes between each search

    //Print the Results
    gw->PrintState(s, WorldNodes->size() * WorldNodes->at(0).at(0)->length, "BFSPlot.gpi");


    controller->PlayGame();
    cout << "DONE WITH GAME!!!" << endl;
}

Position GetStartNode() {
   MyTank curTank = controller->env.myTanks.at(0);
   float xloc = curTank.pos[0];
   float yloc = curTank.pos[1];
   return GetNode(xloc, yloc);
}
Position GetEndNode() {
   //Flag redFlag = controller->env.flags.at(0);
   Base redFlag = controller->env.bases.at(0);
   float xloc = redFlag.corners.at(0).x;
   float yloc = redFlag.corners.at(0).y;
   return GetNode(xloc, yloc);
}
Position GetNode(float xloc, float yloc) {
    int ret[2];
    double length = WorldNodes->at(0).at(0)->length;
    for (int c = 0; c < WorldNodes->size(); c++) {
	if ((WorldNodes->at(0).at(c)->y - yloc) > (0 - length)) {
		ret[1] = c;
		break;
	}
    }
    for (int r = 0; r < WorldNodes->size(); r++) {
	if ((WorldNodes->at(r).at(0)->x - xloc) > (0 - length)) {
		ret[0] = r;
		break;
	}
    }
    Position p(ret[0], ret[1]);
    return p;
}

void DiscretizeWorld(double NodeSize) {
	vector<Constant> constants = controller->env.constants;
	double worldSize;
	for (int i = 0; i < constants.size(); i++) {
		if (constants.at(i).name == "worldsize") {
			worldSize = atof(constants.at(i).value.c_str());
			break;
		}
	}
	if ((int)worldSize % (int)NodeSize != 0) {
		cout << "Could not divide world up evenly into node size." << endl;
		exit(0);
	}
	//WorldNodesSize = (int) (worldSize / NodeSize);
	//Node retBuff[WorldNodesSize][WorldNodesSize];
	//int countR = 0;
	//int countC = 0;
        worldSize = worldSize / 2;
	WorldNodes = new vector<vector<Node*> >();
        for (int x = 0 - worldSize; x < worldSize; x += NodeSize) {
		vector<Node*> tmp;
		for (int y = 0 - worldSize; y < worldSize; y += NodeSize) {
			Node* n = new Node(x, y, NodeSize);
			n->visitable = IsVisitable(n);
			tmp.push_back(n);
		}
		WorldNodes->push_back(tmp);
		//countR++;
	}
	cout << "Created WorldNodes size: " << WorldNodes->size();
	//*worldNodes = retBuff;
}

bool IsVisitable(Node* n) {
    float middleX = n->x + n->length/2;
    float middleY = n->y + n->length/2;
    Point nodeMiddle;
    nodeMiddle.x = middleX;
    nodeMiddle.y = middleY;
    //cout << "middle: " << middleX << ", " << middleY << endl;
    for(int i = 0; i < controller->env.obstacles.size(); i++) {
        Obstacle currObst = controller->env.obstacles.at(i);
        Point bottom = currObst.corners.at(0);
        Point right = currObst.corners.at(1);
        Point top = currObst.corners.at(2);
        Point left = currObst.corners.at(3);

        if(bottom.x == left.x && right.x == top.x) {
            if(nodeMiddle.x >= bottom.x &&
               nodeMiddle.y >= bottom.y &&
               nodeMiddle.x <= top.x &&
               nodeMiddle.y <= top.y)
                return false;
        } else {
            if( (IsBelowLine(nodeMiddle, top, right) && IsAboveLine(nodeMiddle, bottom, right)) ||
                (IsBelowLine(nodeMiddle, top, left) && IsAboveLine(nodeMiddle, bottom, left)))
                return false;
        }
    }
	return true;
}
bool IsBelowLine(Point testPoint, Point linePt1, Point linePt2) {
    float m = GetSlope(linePt1, linePt2);
    float yVal = testPoint.x*m + GetB(linePt1, m);
    float yDist = testPoint.y - yVal;
    if(yDist <= 0)
        return true;
    else
        return false;
}
bool IsAboveLine(Point testPoint, Point linePt1, Point linePt2) {
    float m = GetSlope(linePt1, linePt2);
    float yVal = testPoint.x*m + GetB(linePt1, m);
    float yDist = testPoint.y - yVal;
    if(yDist >= 0)
        return true;
    else
        return false;
}
bool IsLeftOfLine(Point testPoint, Point linePt1, Point linePt2) {
    float m = GetSlope(linePt1, linePt2);
    float xVal = (testPoint.y - GetB(linePt1, m))/m;
    float xDist = testPoint.x - xVal;
    if(xDist <= 0)
        return true;
    else
        return false;
}
bool IsRightOfLine(Point testPoint, Point linePt1, Point linePt2) {
    float m = GetSlope(linePt1, linePt2);
    float xVal = (testPoint.y - GetB(linePt1, m))/m;
    float xDist = testPoint.x - xVal;
    if(xDist >= 0)
        return true;
    else
        return false;
}
float GetSlope(Point p1, Point p2) {
    return (p1.y - p2.y)/(p1.x - p2.x);
}
float GetB(Point p, float slope) {
    return p.y - (slope*p.x);
}
void GnuplotTest() {
/*	cout << "\nPrinting Gnuplot...\n";
	string s = "\n";
	for (int i = 0; i < WorldNodes->size(); i++)
		for (int j = 0; j < WorldNodes->size(); j++)
			s += gw->PrintNode(*WorldNodes->at(i).at(j), 6);
	s += gw->PrintAniData(1);*/
	
/*        Node startNode(-400, -400, 20);
        Node nextNode(-400, -380, 20);
        Node lastNode(-380,-360, 20);
        string s = gw->PrintNode(startNode, 2);
        s += gw->PrintAniData(2);
        s += gw->PrintNode(nextNode, 6);
        s += gw->PrintAniData(2);
        s += gw->PrintLine(startNode, nextNode, -1);
        s += gw->PrintAniData(2);
        s += gw->PrintNode(lastNode, 1);
        s += gw->PrintAniData(2);
        s += gw->PrintLine(nextNode, lastNode, -1);
        s += gw->PrintAniData(2);*/
     //   gw->PrintState(s, WorldNodes->size() * WorldNodes->at(0).at(0)->length, "GnuPlottest.gpi");

}


//------------------------------------------------------
void PrintGnuplotInfo() {
    /*
    vector<Obstacle> obstacles = controller->env.obstacles;
    EnvironmentData *env = &controller->env;

    ofstream output;
    output.open("output.txt");

    output << "\n\nOBSTACLES" << endl << endl;
    output << "(";
    for(int i = 0; i < obstacles.size(); i++) {
        Obstacle currObst = obstacles.at(i);
        output << endl << "(";
        for(int j = 0; j < currObst.corners.size(); j++) {
            Point corner = currObst.corners.at(j);
            if(j != 0)
                output << ", ";
            output << "(" << corner.x << ", " << corner.y << ")";
        }
        output << ")";
        if(i != obstacles.size() -1)
            output << ", ";
    }
    output << ")" << endl;

    output << "\n\nFIELD VALUES" << endl << endl;

    output << "(";
    for(int x = -400; x <= 400; x += 12.5) {
        for(int y = -400; y <= 400; y+= 12.5) {
            float forceX;
            float forceY;
            GenerateField(x,y, &forceX, &forceY, "green", false);
            output << "((" << x << ", " << y << "), (" << forceX << ", " << forceY << "))";
            if(y != 400 || x != 400)
                output << ",\n";
        }
    }
    output << ")";

    output.close();
    */
}
//------------------------------------------------------
void *DummyRobot(void *ptr )
{
	sleep(3); //Let the MyTanks command run first thing to establish number of tanks

    /*
	int *index;
	index = (int *) ptr;
	while (1) {
	   if (controller->speed(*index, 0.5))
			cout << "tank moving!!!" << endl;
		else
			cout << "Couldn't Move Tank!" << endl;

        cout << "DUMMY MY TANKS SIZE: " << controller->env.myTanks.size() << endl;
		MyTank *curTank = &controller->env.myTanks.at(*index);
        vector<Obstacle> *obstacles = &controller->env.obstacles;
       
        int sleepAmount = 200;
        while(1 == 1) {
            if(curTank->status == "dead") { }
            else if(HitObstacle(curTank)) {
                cout << "hit obstacle" << endl;
                RotateDegrees(curTank, *index, curTank->angle, PI/3, true);
            }
            if(curTank->angle != 0)
                controller->angvel(*index, 0);

            usleep(sleepAmount);
            controller->shoot(*index);
        }
	}
    */
}
//------------------------------------------------------
bool HitObstacle(MyTank *tank) {
    //cout << tank->ToString() << endl;
    if(abs(tank->velocity[0]) < 0.09 || abs(tank->velocity[1]) < 0.09)
        return true;
    return false;
}
void RotateDegrees(MyTank *tank, int index, float originalAngle, float amount, bool right) {
    /*
    float goalAngle = Wrap((originalAngle + amount ), 2*PI);

    float error = abs(goalAngle - tank->angle);

    if(right)
        controller->angvel(index, -0.5);
    else
        controller->angvel(index, 0.5);
    while(error > 0.05) {
        usleep(200);
        //cout << "    --> " << tank->ToString() << endl;
        if(index == 1)
            cout << "goal angle: " << goalAngle << "   currAngle: " << tank->angle << endl;
        error = abs(goalAngle - tank->angle);
    }
    controller->angvel(index, 0);
    */
}
//------------------------------------------------------
bool ParseArgs(int argc, char** argv) {
    if(argc < 3 || argc > 4)
        return false;
    
    string currArg = "";
    for(int i = 1; i < argc; i++) {
        currArg = argv[i];
        if(currArg != "-d" && SERVER == "")
            SERVER = currArg;
        else if(currArg != "-d" && STR_PORT == "")
            STR_PORT = currArg;
        else if(currArg == "-d")
            DEBUG = true;
    }
    
    if(!IsNum(STR_PORT)) {
        cout << "ERROR:" << endl
             << "    The given port ("+STR_PORT+") is not a number." << endl
             << "    Go away and try again when you know what a number is." << endl;
        return false;
    }

    PORT = atoi(STR_PORT.c_str());

    if(DEBUG) {
        cout << "DEBUG MODE:" << endl
             << "    Server: " << SERVER << endl
             << "    Port:   " << PORT << endl;
    }
    
    return true;
}
//------------------------------------------------------
bool IsNum(string str) {
    for(int i = 0; i < static_cast<int>(str.length()); i++) {
        if(!isdigit(str.at(i)))
            return false;
    }
    return true;
}
//------------------------------------------------------
void DisplayBanner() {
    cout << endl
         << "          +-------------------------------------------+" << endl
         << "          |               AI for BZFLAG               |" << endl
         << "          +-------------------------------------------+" << endl
         << "                Nephi Johnson & William Armstrong" << endl
         << "                             CS470" << endl
         << endl;
}
//------------------------------------------------------
void DisplayUsage() {
    cout << "USAGE: " << arg0 << " [-d] <server> <port> [NodeSize]" << endl
         << endl
         << "    -d       --    DEBUG MODE" << endl
         << "    server   --    bzrobots server " << endl
         << "    port     --    bzrobots port " << endl
	 << "    NodeSize --    size of nodes used in search algorithm " << endl << endl << endl;
}
//------------------------------------------------------
