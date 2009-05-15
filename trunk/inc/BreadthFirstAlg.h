
#ifndef BREADTH_FIRST_ALG
#define BREADTH_FIRST_ALG


#include "Node.h"
#include "SearchAlg.h"

#include <vector>
#include <string>
#include <string.h>

class BreadthFirstAlg : public SearchAlg {

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:

    private:



  
    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
 
    public:
        BreadthFirstAlg(vector<vector<Node *> > *map);
        void DoSearch(Node *startNode);
        vector<Node *> GetBestPath();

    private:
};


#endif




















