
#ifndef SEARCH_ALG
#define SEARCH_ALG


#include <vector>
#include <string.h>
#include <string>

#include "Node.h"


class SearchAlg {
   

    /*  +--------------+
     *  |  VARIABLES   |
     *  +--------------+ */
    
    public:
        Node[][] *map;

    private:
   


    /*  +--------------+
     *  |  FUNCTIONS   |
     *  +--------------+ */
 
    public:
        explicit SearchAlg();
        virtual void DoSearch(Node *startNode) = 0;
        virtual vector<Node> GetBestPath() = 0;

    private

}



















#endif

























