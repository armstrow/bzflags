
#ifndef ITERATIVE_DEEPENING_ALG
#define ITERATIVE_DEEPENING_ALG


#include "Node.h"
#include "SearchAlg.h"

#include <vector>
#include <string>
#include <string.h>

class IterativeDeepeningAlg : public SearchAlg {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:

    private:



  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
 
    public:
        IterativeDeepeningAlg(vector<vector<Node *> > *map);
        void DoSearch(Node *startNode);
        vector<Node *> GetBestPath();

    private:
};


#endif




















