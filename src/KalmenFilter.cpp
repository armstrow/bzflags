#include "KalmenFilter.h"
#include <stdio.h>
#define DELTA_T 0.5
#define VARIANCE 5
#define NEG_C -0.1

using namespace std;

KalmenFilter::KalmenFilter() {
//Initialize constant matrices

////////
// F
////////
	float accel = DELTA_T * DELTA_T / 2;
	float vals[36] = {1,  DELTA_T, accel,   0,  0,       0,
					  0,  1,       DELTA_T, 0,  0,       0,
					  0,  NEG_C,   1,       0,  0,       0,
					  0,  0,       0,       1,  DELTA_T, accel,
					  0,  0,       0,       0,  1,       DELTA_T,
					  0,  0,       0,       0,  NEG_C,   1};      
	int xSize = 6, ySize = 6;
	F.SetSize(xSize, ySize);
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			F(i, j) = vals[(i*ySize)+j];	
////////////
// SigmaX
////////////
	float vals2[36] ={0.1, 0,   0,   0,   0,   0,
					  0,   0.1, 0,   0,   0,   0,
					  0,   0,   100, 0,   0,   0,
					  0,   0,   0,   0.1, 0,   0,
					  0,   0,   0,   0,   0.1, 0,
					  0,   0,   0,   0,   0,   100};      
	xSize = 6, ySize = 6;
	SigmaX.SetSize(xSize, ySize);
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			SigmaX(i, j) = vals2[(i*ySize)+j];	
////////
// H
////////
	float valsH[12] = {1, 0, 0, 0, 0, 0,
					   0, 0, 0, 1, 0, 0};      
	xSize = 2, ySize = 6;
	H.SetSize(xSize, ySize);
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			H(i, j) = valsH[(i*ySize)+j];
////////////
// SigmaZ
////////////
	float valsZ[4] = {VARIANCE*VARIANCE, 0,
					  0, VARIANCE*VARIANCE};
	xSize = 2, ySize = 2;
	SigmaZ.SetSize(xSize, ySize);
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			SigmaZ(i, j) = valsZ[(i*ySize)+j];		
	
	Ht = ~H;
	Ft = ~F;
////////////
// I
////////////
	float valsI[36] ={1, 0, 0, 0, 0, 0,
					  0, 1, 0, 0, 0, 0,
					  0, 0, 1, 0, 0, 0,
					  0, 0, 0, 1, 0, 0,
					  0, 0, 0, 0, 1, 0,
					  0, 0, 0, 0, 0, 1};      
	xSize = 6, ySize = 6;
	I.SetSize(xSize, ySize);
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			I(i, j) = valsI[(i*ySize)+j];

/***************************
* Non-constant Matrices
***************************/
//Mu
	float valsM[6] = {0,
					  0,
					  0,
					  0,
					  0, 
					  0};
	xSize = 6, ySize = 1;
	Mu.SetSize(xSize, ySize);
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			Mu(i, j) = valsM[(i*ySize)+j];

// SigmaK
	float valsK[36] ={100, 0,   0,   0,   0,   0,
					  0,   0.1, 0,   0,   0,   0,
					  0,   0,   0.1, 0,   0,   0,
					  0,   0,   0,   100, 0,   0,
					  0,   0,   0,   0,   0.1, 0,
					  0,   0,   0,   0,   0,   0.1};      
	xSize = 6, ySize = 6;
	SigmaK.SetSize(xSize, ySize);
	for (int i = 0; i < xSize; i++)
		for (int j = 0; j < ySize; j++)
			SigmaK(i, j) = valsK[(i*ySize)+j];

//Temp  
	Temp = ((F * SigmaK) * Ft) + SigmaX;
//K
	K = (Temp * (Ht * !((H * (Temp * Ht)) + SigmaZ)));
//SigmaK + 1
	SigmaK = (I - (K * H)) * Temp;
}

float* KalmenFilter::update(float ObsX, float ObsY) {
	float* rtrn;
	float tmp[2];
	tmp[0] = 1.0;
	tmp[1] = 1.0;
	rtrn = tmp;
	return rtrn;
}

float* KalmenFilter::predict(int numTimeSteps){
	float* rtrn;
	float tmp[2];
	tmp[0] = 1.0;
	tmp[1] = 1.0;
	rtrn = tmp;
	return rtrn;
}



