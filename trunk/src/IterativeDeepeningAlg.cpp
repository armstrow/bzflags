

#include <vector>

#include "Node.h"
#include "IterativeDeepeningAlg.h"
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

#define DELAY   1





/* +--------------------------------+
 * |             PUBLIC             |
 * +--------------------------------+  */


//------------------------------------------------------
IterativeDeepeningAlg::IterativeDeepeningAlg(vector<vector<Node *> > *map, GnuplotWriter *writer): SearchAlg(map, writer) {
    this->map = map;
    this->gw = writer;
}
//------------------------------------------------------
string IterativeDeepeningAlg::DoSearch(Node startNode, Node endNode) {

}
//------------------------------------------------------
vector<Node *> IterativeDeepeningAlg::GetBestPath() {
    
}
//------------------------------------------------------








/* +--------------------------------+
 * |            PRIVATE             |
 * +--------------------------------+  */























