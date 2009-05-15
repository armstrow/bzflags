

#include <vector>

#include "Node.h"
#include "DepthFirstAlg.h"
#include "SearchAlg.h"





/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */


//------------------------------------------------------
DepthFirstAlg::DepthFirstAlg(vector<vector<Node *> > *map, GnuplotWriter* writer);
    this->map = map;
    this->gw = writer;
}
//------------------------------------------------------
void DepthFirstAlg::DoSearch(Node *startNode) {
    Position pos(startNode.row, startNode.col);
    map->at(pos.row).at(pos.col)->visited = true;
    string s = "";
    s += gw->PrintNode(map->at(pos.row).at(pos.col), GREEN);
    s += gw->PrintNode(map->at(endNode.row).at(endNode.col), RED);
    s += gw->PrintAniData(DELAY);
    while (pos.row != endNode.row && pos.col != endNode.col) {
        s += EnqueueNeighbors(pos);
        if (q.empty()) {
            cout << "Error, goal not found in BFS" << endl;
            return s;
        }
        Position tmp = (Position)q.front();
        pos.set(tmp.row, tmp.col);
        q.pop();
    }
    return s;
}
//------------------------------------------------------
vector<Node *> DepthFirstAlg::GetBestPath() {
    
}
//------------------------------------------------------
string DepthFirstAlg::EnqueueNeighbors(Position p) {
    string s = "";
    s += EnQ(p.row + 1, p.col - 1, p);
    s += EnQ(p.row + 1, p.col, p);
    s += EnQ(p.row + 1, p.col + 1, p);
    s += EnQ(p.row, p.col - 1, p);
    s += EnQ(p.row, p.col + 1, p);
    s += EnQ(p.row - 1, p.col - 1, p);
    s += EnQ(p.row - 1, p.col, p);
    s += EnQ(p.row - 1, p.col + 1, p);
    return s;
}
string DepthFirstAlg::EnQ(int row, int col, Position from) {
    string s = ""; 
    if (map->at(row).at(col)->visitable && !map->at(row).at(col)->visited) {
        map->at(row).at(col)->visited = true;
        Position n(row, col);
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























