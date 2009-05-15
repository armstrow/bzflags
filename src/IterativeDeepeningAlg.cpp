

#include <vector>

#include "Node.h"
#include "IterativeDeepeningAlg.h"
#include "SearchAlg.h"
#include "GnuplotWriter.h"




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























