

#include <vector>

#include "Node.h"
#include "GreedyBestFirstAlg.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"
#include "EnvironmentData.h"
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


/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */
using namespace std;

//------------------------------------------------------
GreedyBestFirstAlg::GreedyBestFirstAlg(vector<vector<Node *> > *map, GnuplotWriter* writer, bool penalize, EnvironmentData *env): SearchAlg(map, writer) {
    this->map = map;
    this->gw = writer;
    this->penalize = penalize;
    this->env = env;
}
//------------------------------------------------------
string GreedyBestFirstAlg::DoSearch(Position startNode, Position endNode) {
    Position pos(startNode.row, startNode.col);
    map->at(pos.row).at(pos.col)->visited = true;

    string s = "";

    s += gw->PrintNode(map->at(pos.row).at(pos.col), GREEN);
    s += gw->PrintNode(map->at(endNode.row).at(endNode.col), RED);
    s += gw->PrintAniData(DELAY);

    while (pos.row != endNode.row || pos.col != endNode.col) {
        s += EnqueueNeighbors(pos, endNode);
        if (q.empty()) {
            cout << "Error, goal not found in BFS" << endl;
            return s;
        }
        Position tmp = (Position)q.top();
        s += gw->PrintLine(map->at(pos.row).at(pos.col), map->at(tmp.row).at(tmp.col), ORANGE);
        s += gw->PrintAniData(DELAY);
        pos.set(tmp.row, tmp.col);
        q.pop();
        cout << "checking node at: " << pos.row << "," << pos.col << endl;
    }
    cout << "Goal found!!!" << endNode.row << "," << endNode.col << endl;
    return s;
    /*
    */
}
//------------------------------------------------------
float GreedyBestFirstAlg::GetHeuristic(int row, int col, Position endNode) {
    float dist = sqrt( (row - endNode.row)*(row - endNode.row) +
                       (col - endNode.col)*(col - endNode.col) );

    float result = 0;

    Node *aNode = map->at(0).at(0);
    float nodeSize = aNode->length;
    int mapWidth = map->size();

    float tankValues = 0;
    float wallValues = 0;
    float obstValues = 0;

    if(penalize) {
        //iterate enemy tanks
        for(int i = 0; i < env->otherTanks.size(); i++) {
            OtherTank otherTank = env->otherTanks.at(i);
            Position pos = GetNode(otherTank.x, otherTank.y);
            float dist = sqrt( (row - pos.row)*(row - pos.row) +
                               (col - pos.col)*(col - pos.col));
            tankValues += (10 - dist < 0) ? 0 : 10 - dist;
        }

        //check the walls
        double horizDist = (row < mapWidth/2) ? row : row - mapWidth/2;
        double vertDist = (col < mapWidth/2) ? col : col - mapWidth/2;
        //wallValues += 
        

        //check obstacles
        for(int i = 0; i < env->obstacles.size(); i++) {
            Obstacle currObst = env->obstacles.at(i);
            float centerX;
            float centerY;
            GetCenter(&centerX, &centerY, currObst.corners);
            Position pos = GetNode(centerX, centerY);
            float dist = sqrt( (row - pos.row)*(row - pos.row) +
                               (col - pos.col)*(col - pos.col) );
            wallValues += dist;
        }
    }

    result += tankValues*2;
    result += wallValues*1.5;
    result += dist;

    return result;
}
//------------------------------------------------------
void GreedyBestFirstAlg::GetCenter(float *centerX, float *centerY, vector<Point> points) {
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
Position GreedyBestFirstAlg::GetNode(float xloc, float yloc) {
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
vector<Node *> GreedyBestFirstAlg::GetBestPath() {
    
}
//------------------------------------------------------
string GreedyBestFirstAlg::EnqueueNeighbors(Position p, Position endNode) {
    string s = "";
    s += EnQ(p.row - 1, p.col - 1, p, endNode);
    s += EnQ(p.row - 1, p.col, p, endNode);
    s += EnQ(p.row - 1, p.col + 1, p, endNode);
    s += EnQ(p.row, p.col + 1, p, endNode);
    s += EnQ(p.row + 1, p.col + 1, p, endNode);
    s += EnQ(p.row + 1, p.col, p, endNode);
    s += EnQ(p.row + 1, p.col - 1, p, endNode);
    s += EnQ(p.row, p.col - 1, p, endNode);
    return s;
}
//------------------------------------------------------
string GreedyBestFirstAlg::EnQ(int row, int col, Position from, Position endNode) {
    string s = ""; 
    //bounds check
    if (row >= map->size() || col >= map->size() || row < 0 || col < 0)
        return s;
    if (map->at(row).at(col)->visitable && !map->at(row).at(col)->visited) {
        map->at(row).at(col)->visited = true;
        Position n(row, col);
        n.heuristic = GetHeuristic(row, col, endNode);
        q.push(n);
        s += gw->PrintNode(map->at(row).at(col), BROWN);
        s += gw->PrintLine(map->at(from.row).at(from.col), map->at(row).at(col), BLACK);
        s += gw->PrintAniData(DELAY);
    }
    return s;
}








/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */























