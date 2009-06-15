#include "KalmenFilter.h"
#include <stdio.h>
#define DELTA_T .1
#define VARIANCE 3
#define NEG_C 0.0

using namespace std;

KalmenFilter::KalmenFilter(EnvironmentData *env) {
	//this->gw = new GnuplotWriter(env);
	//gnuplotString = "";

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
// SigmaX -- how wrong everything might be
////////////
	float vals2[36] ={2, 0,   0,   0,   0,   0,
					  0,   3.5, 0,   0,   0,   0,
					  0,   0,   4,  0,   0,   0,
					  0,   0,   0,   2, 0,   0,
					  0,   0,   0,   0,   3.5, 0,
					  0,   0,   0,   0,   0,   4};      
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
	float valsK[36] ={100,  0,   0,   0,   0,   0,
					  0,   0.1, 0,   0,   0,   0,
					  0,   0,   0.1, 0,   0,   0,
					  0,   0,   0,   100,  0,   0,
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

void KalmenFilter::setInitialTankPos(float X, float Y) {
    Mu(0,0) = X;
    Mu(3,0) = Y;
}

float* KalmenFilter::update(float ObsX, float ObsY) {
	float* rtrn;
	float tmp[2];
	

//Z[k+1]
	Matrix Z;
	float xSize = 2, ySize = 1;
	Z.SetSize(xSize, ySize);
	Z(0,0) = ObsX;
	Z(1,0) = ObsY;
//F*Σk*Ft + Σx
	Temp = ((F * SigmaK) * Ft) + SigmaX;
//K[t+1]
	K = (Temp * (Ht * !((H * (Temp * Ht)) + SigmaZ)));
    //cout << "KALMAN::update: K: " << K << endl;
//Mu[t+1]
	//cout << Mu << endl;
    //cout << "KALMAN::update: K-term: " << K * (Z - H * F * Mu) << endl;
	Mu = F * Mu + K * (Z - H * F * Mu);
    //cout << "KALMAN::update: F: " << F << endl;
//SigmaK[t+1]
	SigmaK = (I - (K * H)) * Temp;
	//cout << SigmaK << endl;
    
	tmp[0] = Mu(0,0);
	tmp[1] = Mu(3,0);

	//gnuplotString += gw->DrawObserved(tmp[0], tmp[1]);

	//cout << "KALMAN::update: observed " << ObsX << "," << ObsY << endl;
	//cout << "KALMAN::        predicted" << tmp[0] << "," << tmp[1] << endl;

	rtrn = tmp;
	return rtrn;
}

float* KalmenFilter::predict(int numTimeSteps){
	float* rtrn;
	float tmp[2];
	Matrix NuMu = Mu;
	for (int i = 0; i < numTimeSteps; i++) {
		NuMu = F * NuMu; //μ[t+1] = F * μ[t]
	}
	tmp[0] = NuMu(0,0);
	tmp[1] = NuMu(3,0);
	float ObsX = tmp[0];
	float ObsY = tmp[1];
	//cout << "KALMEN::        predicted" << tmp[0] << "," << tmp[1] << endl;
	////gnuplotString += gw->DrawObserved(ObsX, ObsY);
	////gnuplotString += gw->DrawObserved(ObsX + 5, ObsY);
	////gnuplotString += gw->DrawObserved(ObsX + 5, ObsY + 5);
	////gnuplotString += gw->DrawObserved(ObsX, ObsY+5);
	////gnuplotString += gw->PrintAniData(0);
	////gnuplotString += gw->DrawPredicted(tmp[0], tmp[1], 0.3);  	////////////////////////////////////////////////////////FIX THIS!!!!!!!!!!!!!!!!
	   
    //get how much we trust the position in the x direction
    //float sigmaX = SigmaK(0,0);
    //get how much we trust the position in the y direction
    //float sigmaY = SigmaK(3, 3);
    //get how much we need to stretch the oval along the slope (the correlation btwn sigmaX and Y)
    //float rho = (SigmaK(0, 3)/sigmaX)/sigmaY;
    //float rho = ???//not sure where to get this one from, maybe from SigmaK, but the x and y velocities?
    //string randStr = "";
    //char buff[1000];
    //sprintf(buff, "KalmenFilter%d.gpi", rand());

    //gnuplotString += gw->DrawPredicted(sigmaX, sigmaY, rho);
    
    //gw->PrintState(//gnuplotString, 800, buff);
	rtrn = tmp;
	return rtrn;
}



