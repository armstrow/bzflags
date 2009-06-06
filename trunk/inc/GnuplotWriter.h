#ifndef GNUPLOT_WRITER
#define GNUPLOT_WRITER


#include <string.h>
#include <fstream>
#include "Obstacle.h"
#include "EnvironmentData.h"
#include "Node.h"

class GnuplotWriter{
    /**************
    * Variables
    **************/
    private:
	EnvironmentData* env;

    public:


   /**************
   * Functions
   **************/

    private:

	string PrintHeaderData(double size);
	string PrintObstacleData(vector<Obstacle> obstacles);
	//plots anything else that needs to be ploted that did not get ploted elsewhere
	string PrintFooterData();
	string PrintLine(double startX, double startY, double endX, double endY, int color);

    public:
	GnuplotWriter(EnvironmentData* env);
	/*   ...
	   string output;
	   ...
	   output+= PrintLine(currentNode, neighborNode, color);
	   output+= PrintAniData(delay);
	*/
	//Prints a line from the center of current to the center of neighbor
	string PrintLine(Node* current, Node* neighbor, int color);

	string PrintNode(Node* n, int color);
	
	//Prints the rest of the command, including the delay for the animation
	string PrintAniData(double delay);

	/*   ...
	   //within the search code, at the end you can push the path data to a file.
	   PrintState(output, "AStar.gpi");
	   ...
	*/
	string DrawObserved(float x, float y);
	string DrawPredicted(float sigma_x, float sigma_y, float rho);

	void PrintState(string s, double size, char * filename);
    string ClearScreen();
};

#endif
