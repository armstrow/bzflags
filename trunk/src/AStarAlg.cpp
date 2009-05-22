

#include <vector>

#include "Node.h"
#include "AStarAlg.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"
#include <math.h>


#define BLACK  -1
#define RED     1
#define GREEN   2
#define BLUE    3
#define MAGENTA 4
#define AQUA    5
#define BROWN   6
#define YELLOW  7
#define ORANGE  8

#define DELAY   0.1

#define TANK_PENALTY_SIZE 2
#define OBST_PENALTY_SIZE 1.5
#define TANK_PENALTY_FAC 5
#define OBST_PENALTY_FAC 5


/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */
using namespace std;

//------------------------------------------------------
AStarAlg::AStarAlg(vector<vector<Node *> > *map, GnuplotWriter* writer, bool penalized, EnvironmentData *env): SearchAlg(map, writer) {
    this->map = map;
    this->penalize = penalized;
    this->env = env;
    this->gw = new GnuplotWriter(env);
}
//------------------------------------------------------
string AStarAlg::DoSearch(Position startNode, Position endNode) {
    string result = "";
    return result;
}
//------------------------------------------------------
string AStarAlg::DoSearch(Position startNode, Position endNode, vector<Position> *finalPath) {
    cout << "IN ASTAR* : DOSEARCH" << endl;
    cout << " START NODE: "; cout << startNode.ToString() << endl;
    cout << " END NODE:   "; cout << endNode.ToString() << endl;
	while (!q.empty()) {
	    q.pop();
	}    
    Position p = startNode;
    Position* pos = &p;
    map->at(pos->row).at(pos->col)->visited = true;

    string s = "";

    //s += gw->PrintNode(map->at(pos->row).at(pos->col), GREEN);
    //s += gw->PrintNode(map->at(endNode.row).at(endNode.col), RED);
    //s += gw->PrintAniData(DELAY);

    int nodesPopped = 0;
    while (pos->row != endNode.row || pos->col != endNode.col) {
        s += EnqueueNeighbors(pos, endNode);
        if (q.empty()) {
        	s += EnqueueNeighbors(pos, endNode);
            return s;
            //break;
        }
        while (map->at(q.top().row).at(q.top().col)->visited == true) {
            q.pop();
        }
        map->at(q.top().row).at(q.top().col)->visited = true;
        //s += gw->PrintLine(map->at(pos->row).at(pos->col), map->at(q.top().row).at(q.top().col), ORANGE);
        ////s += gw->PrintAniData(DELAY);
        pos = (Position *)&q.top();
        q.pop();
        nodesPopped++;
    }
    //cout << "              NODES POPPED: " << nodesPopped << endl;

    Position *lastPos = pos;
    float pathCost = 0;
    int pathLength = 0;
    finalPath->clear();
    while(lastPos->col != startNode.col || lastPos->row != startNode.row) {
        cout << "in loop" << endl;
        Position copy = *lastPos;
        copy.from = NULL;
        finalPath->push_back(copy);
        ////s += gw->PrintLine(map->at(lastPos->row).at(lastPos->col), map->at(lastPos->from->row).at(lastPos->from->col), GREEN);
        pathCost += lastPos->heuristic;
        lastPos = lastPos->from;
        pathLength++;
    }
    //cout << "              PATH COST  : " << pathCost << endl;
    //cout << "              PATH LENGTH: " << pathLength << endl;

    //cout << "Goal found!!!" << endNode.row << "," << endNode.col << endl;
    ////s += gw->PrintAniData(0);
    return s;
}
//------------------------------------------------------
void AStarAlg::GetCenter(float *centerX, float *centerY, vector<Point> points) {
    float tempX = 0;
    float tempY = 0;
    for(int i = 0; i < points.size(); i++) {
        Point p = points.at(i);
        tempX += p.x;
        tempY += p.y;
    }
    tempX *= .25;
    tempY *= .25;
    *centerX = tempX;
    *centerY = tempY;
}
//------------------------------------------------------
Position AStarAlg::GetNode(float xloc, float yloc) {
    int ret[2];
    double length = map->at(0).at(0)->length;
    for (int c = 0; c < map->size(); c++) {
        if ((map->at(0).at(c)->y - yloc) > (0 - length)) {
            ret[1] = c;
            break;
        }
    }
    for (int r = 0; r < map->size(); r++) {
        if ((map->at(r).at(0)->x - xloc) > (0 - length)) {
            ret[0] = r;
            break;
        }
    }
    Position p(ret[0], ret[1]);
    return p;
}
//------------------------------------------------------
float AStarAlg::GetRealDistance(Position start, Position end) {
	float ns = map->at(start.row).at(start.col)->length / 2;	
    float x = map->at(start.row).at(start.col)->x + ns;
    float y = map->at(start.row).at(start.col)->y + ns;
    float endX = map->at(end.row).at(end.col)->x + ns;
    float endY = map->at(end.row).at(end.col)->y + ns;
    float dist = sqrt( (x - endX)*(x - endX) +
            (y - endY)*(y - endY) );
    return dist;
}
//------------------------------------------------------
float AStarAlg::GetHeuristic(int row, int col, Position endNode) {
	Position start(row, col);    
    float dist = GetRealDistance(start, endNode);
    
    float result = 0;

    Node *aNode = map->at(0).at(0);
    float nodeSize = aNode->length;
    int mapWidth = map->size();
    int mapSize = mapWidth * map->at(0).at(0)->length;

    float tankValues = 0;
    float wallValues = 0;
    float obstValues = 0;

    if(penalize) {
        //iterate enemy tanks
        vector<OtherTank> otherTanks = env->getOtherTanks();
        for(int i = 0; i < otherTanks.size(); i++) {
            OtherTank otherTank = otherTanks.at(i);
            Position pos = GetNode(otherTank.x, otherTank.y);
            float dist = GetRealDistance(start, pos);
            float radius = nodeSize * TANK_PENALTY_SIZE;
            float offBy = (radius - dist < 0) ? 0 : radius - dist;
            tankValues += offBy * TANK_PENALTY_FAC;
            //if (offBy > 0) cout << "Tank Penalty Added!!!!!!!" << offBy << endl;
        }

        //check the walls
        double horizDist = (row < mapWidth/2) ? row : row - mapWidth/2;
        double vertDist = (col < mapWidth/2) ? col : col - mapWidth/2;
        /*
        */

        //check obstacles
        /*
        vector<Obstacle> obstacles = env->getObstacles();
        for(int i = 0; i < obstacles.size(); i++) {
            Obstacle currObst = obstacles.at(i);
            float centerX;
            float centerY;
            GetCenter(&centerX, &centerY, currObst.corners);
            Position pos = GetNode(centerX, centerY);
            float dist = GetRealDistance(start, pos);
            Position corner = GetNode(currObst.corners.at(0).x, currObst.corners.at(0).y);
            float radius = GetRealDistance(pos, corner) * OBST_PENALTY_SIZE;
            float offBy = (radius - dist < 0) ? 0 : radius - dist;
            wallValue//s += offBy * OBST_PENALTY_FAC;
            //if (offBy > 0) cout << "Obstacle Penalty Added!!!!!!!" << offBy << endl;
        }
        */
    }

    result += tankValues;
    result += wallValues;
    result += dist;

    return result;
}
//------------------------------------------------------
vector<Node *> AStarAlg::GetBestPath() {
    
}

//------------------------------------------------------
string AStarAlg::EnqueueNeighbors(Position* p, Position endNode) {
    string s = "";
    //s += EnQ(p->row - 1, p->col - 1, p, endNode);
    //s += EnQ(p->row - 1, p->col, p, endNode);
    //s += EnQ(p->row - 1, p->col + 1, p, endNode);
    //s += EnQ(p->row, p->col + 1, p, endNode);
    //s += EnQ(p->row + 1, p->col + 1, p, endNode);
    //s += EnQ(p->row + 1, p->col, p, endNode);
    //s += EnQ(p->row + 1, p->col - 1, p, endNode);
    //s += EnQ(p->row, p->col - 1, p, endNode);
    return s;
}
//------------------------------------------------------
string AStarAlg::EnQ(int row, int col, Position* from, Position endNode) {
    string s = ""; 
    //bounds check
    //cout << "Enqueing: " << row << "," << col << endl;
    if (row >= map->size() || col >= map->size() || row < 0 || col < 0)
        return s;
    if (map->at(row).at(col)->visitable) {        
        float ns = map->at(row).at(col)->length / 2;
        float x = map->at(row).at(col)->x + ns;
        float y = map->at(row).at(col)->y + ns;
        float fromX = map->at(from->row).at(from->col)->x + ns;
        float fromY = map->at(from->row).at(from->col)->y + ns;
        float dist = sqrt( (x - fromX)*(x - fromX) +
                (y - fromY)*(y - fromY) ) + from->distSoFar;
        Position n(row, col, dist + GetHeuristic(row, col, endNode), dist, from);
        //cout << "Enqueued Node: (" << row << "," << col << "):" << n.heuristic << " = " << dist << " + " << GetHeuristic (row, col, endNode) << endl;
        q.push(n);
        //cout << "Enqueued Node: (" << row << "," << col << "):" << n.heuristic << " = " << dist << " + " << GetHeuristic (row, col, endNode) << endl;
        //s += gw->PrintNode(map->at(row).at(col), BROWN);
        //s += gw->PrintLine(map->at(from->row).at(from->col), map->at(row).at(col), BLACK);
        ////s += gw->PrintAniData(DELAY);
    }
    return s;
}








/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */























