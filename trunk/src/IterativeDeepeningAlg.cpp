

#include <vector>

#include "IterativeDeepeningAlg.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"
#include <string>
#include <string.h>
#include "Node.h"

#define BLACK  -1
#define RED     1
#define GREEN   2
#define BLUE    3
#define MAGENTA 4
#define AQUA    5
#define BROWN   6
#define YELLOW  7
#define ORANGE  8

#define DELAY   0.05


using namespace std;


/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */


//------------------------------------------------------
IterativeDeepeningAlg::IterativeDeepeningAlg(vector<vector<Node *> > *map, GnuplotWriter *writer): SearchAlg(map, writer) {
    this->map = map;
    this->gw = writer;
}
//------------------------------------------------------
string IterativeDeepeningAlg::DoSearch(Position startNode, Position endNode) {
    Position pos(startNode.row, startNode.col);
    pos.level = 0;
    map->at(pos.row).at(pos.col)->visited = true;
    string s = "";
    s += gw->PrintNode(map->at(pos.row).at(pos.col), GREEN);
    s += gw->PrintNode(map->at(endNode.row).at(endNode.col), RED);
    //s += gw->PrintAniData(DELAY);

    int maxDepth = 1;

    Position *currPos = &pos;
    int nodesPopped = 0;
    while ((currPos->row != endNode.row || currPos->col != endNode.col)) {
        if(currPos->level <= maxDepth){
            s += EnqueueNeighbors(currPos);
        }

        if(q.empty()) {

            while(!q.empty())
                q.pop();

            for(int i = 0; i < map->size(); i++)
                for(int j = 0; j < map->at(i).size(); j++)
                    map->at(i).at(j)->visited = false;

            maxDepth += 1;
            currPos = &pos;
            continue;
        }

        if (q.empty()) {
            cout << "Error, goal not found in DFS" << endl;
            return s;
        }

        Position *tmp = q.top();
        currPos = tmp;
        q.pop();
        nodesPopped++;
        //cout << "checking node at: " << currPos->row << "," << currPos->col << endl;
    }
    cout << "              NODES POPPED: " << nodesPopped << endl;

    Position *lastPos = currPos;
    int pathLength = 0;
    while(lastPos->col != startNode.col || lastPos->row != startNode.row) {
        cout << "in loop" << endl;
        s += gw->PrintLine(map->at(lastPos->row).at(lastPos->col), map->at(lastPos->from->row).at(lastPos->from->col), GREEN);
        s += gw->PrintAniData(0);
        lastPos = lastPos->from;
        pathLength++;
    }
    cout << "              PATH LENGTH: " << pathLength << endl;

    return s;
}
//------------------------------------------------------
vector<Node *> IterativeDeepeningAlg::GetBestPath() {
    
}
//------------------------------------------------------
string IterativeDeepeningAlg::EnqueueNeighbors(Position *p) {
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
string IterativeDeepeningAlg::EnQ(int row, int col, Position *from) {
    string s = ""; 
    //bounds check
    if (row >= map->size() || col >= map->size() || row < 0 || col < 0)
        return s;
    if (map->at(row).at(col)->visitable && !map->at(row).at(col)->visited) {
        map->at(row).at(col)->visited = true;
        Position *n = new Position(row, col);
        n->level = from->level + 1;
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























