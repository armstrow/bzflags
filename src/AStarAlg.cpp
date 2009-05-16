

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
float AStarAlg::GetHeuristic(int row, int col, Position endNode) {
    float dist = sqrt( (row - endNode.row)*(row - endNode.row) +
                       (col - endNode.col)*(col - endNode.col) );
    return dist;
}
//------------------------------------------------------
vector<Node *> AStarAlg::GetBestPath() {
    
}
//------------------------------------------------------
string AStarAlg::EnqueueNeighbors(Position p, Position endNode) {
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
string AStarAlg::EnQ(int row, int col, Position from, Position endNode) {
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






















