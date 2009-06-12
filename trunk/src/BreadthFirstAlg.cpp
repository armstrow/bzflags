

#include <vector>

#include "Node.h"
#include "BreadthFirstAlg.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"


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
BreadthFirstAlg::BreadthFirstAlg(vector<vector<Node *> > *map, GnuplotWriter* writer): SearchAlg(map, writer) {
    this->map = map;
    this->gw = writer;
}
//------------------------------------------------------
string BreadthFirstAlg::DoSearch(Position startNode, Position endNode) {
    Position pos(startNode.row, startNode.col);
    map->at(pos.row).at(pos.col)->visited = true;
    string s = "";
    s += gw->PrintNode(map->at(pos.row).at(pos.col), GREEN);
    s += gw->PrintNode(map->at(endNode.row).at(endNode.col), RED);
    s += gw->PrintAniData(DELAY);

    Position *currPos = &pos;
    int nodesPopped = 0;
    while (currPos->row != endNode.row || currPos->col != endNode.col) {
        s += EnqueueNeighbors(currPos);
        if (q.empty()) {
            //cout << "Error, goal not found in BFS" << endl;
            return s;
        }
        Position *tmp = q.front();
        s += gw->PrintLine(map->at(currPos->row).at(currPos->col), map->at(tmp->row).at(tmp->col), ORANGE);
        //s += gw->PrintAniData(0);
        currPos = tmp;
        q.pop();
        nodesPopped++;
        ////cout << "checking node at: " << currPos->row << "," << currPos->col << endl;
    }

    //cout << "              NODES POPPED: " << nodesPopped << endl;
    Position *lastPos = currPos;
    int pathLength = 0;
    while(lastPos->col != startNode.col || lastPos->row != startNode.row) {
        //cout << "in loop" << endl;
        s += gw->PrintLine(map->at(lastPos->row).at(lastPos->col), map->at(lastPos->from->row).at(lastPos->from->col), GREEN);
        lastPos = lastPos->from;
        pathLength++;
    }
    //cout << "              PATH LENGTH: " << pathLength << endl;

    //cout << "Goal found!!!" << endNode.row << "," << endNode.col << endl;
    s += gw->PrintAniData(0);
    return s;
}
//------------------------------------------------------
vector<Node *> BreadthFirstAlg::GetBestPath() {
    
}
//------------------------------------------------------
string BreadthFirstAlg::EnqueueNeighbors(Position *p) {
    string s = "";
    s += EnQ(p->row - 1, p->col - 1, p);
    s += EnQ(p->row - 1, p->col, p);
    s += EnQ(p->row - 1, p->col + 1, p);
    s += EnQ(p->row, p->col + 1, p);
    s += EnQ(p->row + 1, p->col + 1, p);
    s += EnQ(p->row + 1, p->col, p);
    s += EnQ(p->row + 1, p->col - 1, p);
    s += EnQ(p->row, p->col - 1, p);
    return s;
}
//------------------------------------------------------
string BreadthFirstAlg::EnQ(int row, int col, Position *from) {
    string s = ""; 
    //bounds check
    if (row >= map->size() || col >= map->size() || row < 0 || col < 0)
	return s;
    if (map->at(row).at(col)->visitable && !map->at(row).at(col)->visited) {
        map->at(row).at(col)->visited = true;
        Position *n = new Position(row, col);
        n->from = from;
        q.push(n);
        s += gw->PrintNode(map->at(row).at(col), BROWN);
        s += gw->PrintLine(map->at(from->row).at(from->col), map->at(row).at(col), BLACK);
        //s += gw->PrintAniData(DELAY);
    }
    return s;
}
//------------------------------------------------------








/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */























