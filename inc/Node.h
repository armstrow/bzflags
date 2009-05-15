#ifndef NODE
#define NODE

class Node{
    public:
	//Representing the lower left corner and side length
	double x, y, length;
	bool visitable;
        bool visited;

	Node(double lowerX, double lowerY, double sideLength) {
		x = lowerX;
		y = lowerY;
		length = sideLength;
		visitable = true;
		visited = false;
	}
};


#endif
