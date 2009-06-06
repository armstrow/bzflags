#include "GnuplotWriter.h"


using namespace std;

string GnuplotWriter::PrintHeaderData(double size) {
   char buff[1000];;
   string s;
   double sz = size / 2;
   sprintf (buff, "set title \"Plot\"\nset xrange [-%f: %f]\nset yrange [-%f: %f]\nunset key\nset size square\n", sz, sz, sz, sz);
   s += buff;
   return s;
}
  
GnuplotWriter::GnuplotWriter(EnvironmentData* environment) {
	env = environment;
}

string GnuplotWriter::PrintObstacleData(vector<Obstacle> obstacles) {
   char buff[100];
   string s;
   for (int i = 0; i < (int)obstacles.size(); i++) {
      double x1;
      double x2;
      double y1;
      double y2;
      Obstacle o= obstacles.at(i);
      for (int j = 0; j < 4; j++) {
          x1 = o.corners.at(j).x;
          y1 = o.corners.at(j).y;
          x2 = o.corners.at((j+1)%4).x;
          y2 = o.corners.at((j+1)%4).y;
          sprintf(buff, "set arrow from %f, %f to %f, %f nohead lt 3\n", x1,y1,x2,y2);
          s+=buff;
      }
   }
   return s;	
}

string GnuplotWriter::PrintLine(double startX, double startY, double endX, double endY, int color) {
    char buff[100];
    string s;
    sprintf(buff, "set arrow from %f, %f to %f, %f nohead lt %d\n", startX,startY,endX,endY, color);
    s+=buff;
    return s;
}

   //Prints the rest of the command, including the delay for the animation
string GnuplotWriter::PrintAniData(double delay){
    string s;
    char buff[100];
    sprintf(buff, "\nplot '-' with lines\n0 0 0 0\ne\npause %f\n",delay);
    s+=buff;
    return s;
}

string GnuplotWriter::PrintLine(Node* current, Node* neighbor, int color) {
    string s = PrintLine(current->x + current->length / 2, current->y + current->length / 2, 
			neighbor->x + neighbor->length / 2, neighbor->y + current->length /2,
			color);
    return s;
}

string GnuplotWriter::PrintNode(Node* n, int color) {
	 //cout << "Printing Node: (" << n->x << "," << n->y << "): " << n->length << endl;
    string s =	PrintLine(n->x, n->y, n->x + n->length, n->y, color);
    s += PrintLine(n->x+n->length, n->y, n->x+n->length, n->y+n->length, color);
    s += PrintLine(n->x+n->length, n->y+n->length, n->x, n->y+n->length, color);
    s += PrintLine(n->x, n->y+n->length, n->x, n->y, color);
    return s;
}

//plots anything else that needs to be ploted that did not get ploted elsewhere
string GnuplotWriter::PrintFooterData() {
	return "\n";
}
string GnuplotWriter::ClearScreen() {
    return "\nclear\n";
}
void GnuplotWriter::PrintState(string s, double size, char * filename) {
    printf("Writing Plot Data to File: %s\n", filename);
    ofstream myfile;
    myfile.open(filename);
    myfile << PrintHeaderData(size);
    myfile << PrintObstacleData(env->getObstacles());
    myfile << s;
    //myfile << PrintTankData();
    //myfile << PrintNode(getGoalNode(), 1);
    //myfile << PrintNode(getStartNode(), 2);
    myfile << PrintFooterData();
    myfile.close();
    printf("Finished writing plot data.\n");
}


string GnuplotWriter::DrawObserved(float x, float y) {
	float edgeSize = 5;
	string s =	PrintLine(x, y, x + edgeSize, y, 1);
    s += PrintLine(x+edgeSize, y, x+edgeSize, y+edgeSize, 1);
    s += PrintLine(x+edgeSize, y+edgeSize, x, y+edgeSize, 1);
    s += PrintLine(x, y+edgeSize, x, y, 1);
	return s;
}

string GnuplotWriter::DrawPredicted(float sigma_x, float sigma_y, float rho) {
	string s = "";

	s += "set palette model RGB functions 1-gray, 1-gray, 1-gray\n";
	s += "set isosamples 100\n";
    char buff[100];
    sprintf(buff, "sigma_x = %f\nsigma_y = %f\nrho = %f\n", sigma_x,sigma_y,rho);
    s+=buff;
	s += "\nsplot 1.0/(2.0 * pi * sigma_x * sigma_y * sqrt(1 - rho**2) )* exp(-1.0/2.0 * (x**2 / sigma_x**2 + y**2 / sigma_y**2 - 2.0*rho*x*y/(sigma_x*sigma_y) ) ) with pm3d\n";

	return s;
}





