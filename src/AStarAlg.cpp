

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


/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */
using namespace std;

//------------------------------------------------------
AStarAlg::AStarAlg(vector<vector<Node *> > *map, GnuplotWriter* writer): SearchAlg(map, writer) {
    this->map = map;
    this->gw = writer;
}
//------------------------------------------------------
string AStarAlg::DoSearch(Position startNode, Position endNode) {
    Position p = startNode;
    Position* pos = &p;
    map->at(pos->row).at(pos->col)->visited = true;

    string s = "";

    s += gw->PrintNode(map->at(pos->row).at(pos->col), GREEN);
    s += gw->PrintNode(map->at(endNode.row).at(endNode.col), RED);
    s += gw->PrintAniData(DELAY);

    while (pos->row != endNode.row || pos->col != endNode.col) {
        s += EnqueueNeighbors(pos, endNode);
        if (q.empty()) {
            cout << "Error, goal not found in BFS" << endl;
            return s;
        }
        Position *tmp = q.top();
        while (map->at(tmp->row).at(tmp->col)->visited == true) {
        		q.pop();
        		tmp = q.top();
        }
        map->at(tmp->row).at(tmp->col)->visited = true;
        s += gw->PrintLine(map->at(pos->row).at(pos->col), map->at(tmp->row).at(tmp->col), ORANGE);
        s += gw->PrintAniData(DELAY);
        pos = tmp;
        q.pop();
        cout << "checking node at: " << pos->row << "," << pos->col << endl;
    }

    Position *lastPos = pos;
    while(lastPos->col != startNode.col || lastPos->row != startNode.row) {
        cout << "in loop" << endl;
        s += gw->PrintLine(map->at(lastPos->row).at(lastPos->col), map->at(lastPos->from->row).at(lastPos->from->col), GREEN);
        s += gw->PrintAniData(0);
        lastPos = lastPos->from;
    }

    cout << "Goal found!!!" << endNode.row << "," << endNode.col << endl;
    return s;
    /*
    */
}
//------------------------------------------------------
float AStarAlg::GetHeuristic(int row, int col, Position endNode) {
    float ns = map->at(row).at(col)->length / 2;	
    float x = map->at(row).at(col)->x + ns;
    float y = map->at(row).at(col)->y + ns;
    float endX = map->at(endNode.row).at(endNode.col)->x + ns;
    float endY = map->at(endNode.row).at(endNode.col)->y + ns;
    float dist = sqrt( (x - endX)*(x - endX) +
            (y - endY)*(y - endY) );
    return dist;
}
//------------------------------------------------------
vector<Node *> AStarAlg::GetBestPath() {
    
}

//------------------------------------------------------
string AStarAlg::EnqueueNeighbors(Position* p, Position endNode) {
    string s = "";
    s += EnQ(p->row - 1, p->col - 1, p, endNode);
    s += EnQ(p->row - 1, p->col, p, endNode);
    s += EnQ(p->row - 1, p->col + 1, p, endNode);
    s += EnQ(p->row, p->col + 1, p, endNode);
    s += EnQ(p->row + 1, p->col + 1, p, endNode);
    s += EnQ(p->row + 1, p->col, p, endNode);
    s += EnQ(p->row + 1, p->col - 1, p, endNode);
    s += EnQ(p->row, p->col - 1, p, endNode);
    return s;
}
//------------------------------------------------------
string AStarAlg::EnQ(int row, int col, Position* from, Position endNode) {
    string s = ""; 
    //bounds check
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
        Position *n = new Position(row, col, dist + GetHeuristic(row, col, endNode), dist, from);
        /*n.distSoFar = dist;
          n.from = from; 
          n.heuristic = dist + GetHeuristic(row, col, endNode);*/
        cout << "Enqueued Node: (" << row << "," << col << "):" << n->heuristic << " = " << dist << " + " << GetHeuristic (row, col, endNode) << endl;
        q.push(n);
        s += gw->PrintNode(map->at(row).at(col), BROWN);
        s += gw->PrintLine(map->at(from->row).at(from->col), map->at(row).at(col), BLACK);
        s += gw->PrintAniData(DELAY);
    }
    return s;
}








/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */























