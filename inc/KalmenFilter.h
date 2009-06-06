#ifndef KALMEN_FILTER
#define KALMEN_FILTER

#include "matrix.h"
#include "GnuplotWriter.h"
#include "EnvironmentData.h"

//#ifndef _NO_NAMESPACE
//using namespace std;
using namespace math;
//#define STD std
//#else
//#define STD
//#endif

//#ifndef _NO_TEMPLATE
typedef matrix<double> Matrix;
//#else
//typedef matrix Matrix;
//#endif

class KalmenFilter {

	/******************
	* Variables
	*******************/
	public:


	private:
		//Constant Matrices
		Matrix F, Ft, SigmaX, SigmaZ, H, Ht, I;
		//Non-Constant matrices
		Matrix Mu, SigmaK, K, Temp;
		GnuplotWriter * gw;
		string gnuplotString;
	

	/******************
	* Functions
	*******************/
	public:
		KalmenFilter(EnvironmentData *env);
		float* update(float ObsX, float ObsY);
		float* predict(int numTimeSteps);
	
	private:
};
#endif
