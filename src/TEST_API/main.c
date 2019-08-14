// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

#include "srs_api.h"

#define difftimens(ts2, ts1) (double)ts2.tv_sec - (double)ts1.tv_sec + ((double)ts2.tv_nsec - (double)ts1.tv_nsec)/1.e9

void printResSpx(SRS_PROBLEM * srsProb) {
	double objVal = -1.0;
	int nbIter = -1;
	double * xVals = malloc(2*sizeof(double));
	SRSgetobjval(srsProb, &objVal);
	SRSgetspxitercount(srsProb, &nbIter);
	SRSgetx(srsProb, &xVals);
	printf("Objective value : %lf, nb iter %d, x=%f y=%f\n", objVal, nbIter, xVals[0], xVals[1]);
	free(xVals);
}

int main(int argc, char ** argv) {
	SRS_PROBLEM * srsProb = NULL;

	bool hotstart = false;
	// *** read MPS MIP ***
	if (argc >= 2) {
		for (int i = 1; i < argc; ++i) {
			if (strcmp(argv[i], "hotstart") == 0) { hotstart = true; ++i; }

			if (!hotstart) {
				if (srsProb != NULL) SRSfreeprob(srsProb);
				srsProb = SRScreateprob();
			}
			printf("%s ", argv[i]);
			SRSreadmpsprob(srsProb, argv[i]);
			SRSsetintparams(srsProb, SRS_PARAM_VERBOSE_PNE, 1);

			struct timespec beginTime;
			timespec_get(&(beginTime), TIME_UTC);
			
			SRSoptimize(srsProb);

			struct timespec endTime;
			timespec_get(&(endTime), TIME_UTC);

			double temps = difftimens(endTime, beginTime);

			double objVal = -1.0;
			int nbIter = -1;
			SRSgetobjval(srsProb, &objVal);
			printf("Objective value : %lf, %lf\n", objVal, temps);
		}

		SRSfreeprob(srsProb);
		exit(0);
	}

	srsProb = SRScreateprob();
	SRSfreeprob(srsProb);
	srsProb = SRScreateprob();
	
#define nbCols 2
#define nbRows 2
	
	{
		double obj[nbCols] = { 1.0, 2.0 };
		int colTypes[nbCols] = { SRS_CONTINUOUS_VAR, SRS_CONTINUOUS_VAR };
		double lb[nbCols] = { -1.0, -2.0 };
		double ub[nbCols] = { 1.0, 2.0 };
		char * colNames[nbCols] = { "X1", "x2" };

		double rhs[nbRows] = { 0.5, 0.5 };
		double range[nbRows] = { 0.0, 0.0 };
		char sense[nbRows] = { SRS_GREATER_THAN, SRS_LESSER_THAN };
		char * rowNames[nbRows] = { "c1", "C2" };

		int rowDebInds[2] = { 0, 2 };
		int nbCoefPerRow[2] = { 2, 2 };
		int colInds[4] = { 0, 1, 0, 1 };
		double coefs[4] = { 0.5, 1.0, 1.0, 0.5 };

		SRScreatecols(srsProb, nbCols, obj, colTypes, lb, ub, colNames);
		SRScreaterows(srsProb, nbRows, rhs, range, sense, rowNames);
		SRSsetcoefs(srsProb, rowDebInds, nbCoefPerRow, colInds, coefs);

		SRSoptimize(srsProb);
		printResSpx(srsProb);
		

		// HOTSTART
		int rowIndexes[1] = {0};
		double newRhs[1] = {0.7};
		SRSchgrhs(srsProb, 1, rowIndexes, newRhs);
		
		SRSoptimize(srsProb);
		printResSpx(srsProb);

		SRSfreeprob(srsProb);
	}

	{
		srsProb = SRScreateprob();
		double obj[nbCols] = { 1.0, 1.0 };
		int colTypes[nbCols] = { SRS_CONTINUOUS_VAR, SRS_CONTINUOUS_VAR };
		double lb[nbCols] = { -1.5, -2.5 };
		double ub[nbCols] = { 1.0, 2.0 };
		char * colNames[nbCols] = { "x", "y" };

		double rhs[1] = { SRS_infinite };
		double range[1] = { 0.0};
		char sense[nbRows] = { SRS_LESSER_THAN };
		char * rowNames[nbRows] = { "c1"};

		int rowDebInds[1] = { 0 };
		int nbCoefPerRow[1] = { 1 };
		int colInds[1] = { 0 };
		double coefs[1] = { 0.1 };

		SRScreatecols(srsProb, nbCols, obj, colTypes, lb, ub, colNames);
		SRScreaterows(srsProb, 1, rhs, range, sense, rowNames);
		SRSsetcoefs(srsProb, rowDebInds, nbCoefPerRow, colInds, coefs);
		SRSsetintparams(srsProb, SRS_PARAM_MAXIMIZE, 1);

		SRSoptimize(srsProb);
		printResSpx(srsProb);

		SRSfreeprob(srsProb);
	}

	// *** MIP ***
	srsProb = SRScreateprob();

	double obj[nbCols] = { 2.0, 1.0 };
	int colTypesMip[nbCols] = { SRS_INTEGER_VAR, SRS_INTEGER_VAR };
	double lbMip[nbCols] = { 0. , 0. };
	double ubMip[nbCols] = { 1. , 1. };
	char * colNames[nbCols] = { "X1", "x2" };

	double rhsMip[nbRows] = { 1., 0.5 };
	double range[nbRows] = { 0.0, 0.0 };
	char sense[nbRows] = { SRS_GREATER_THAN, SRS_LESSER_THAN };
	char * rowNames[nbRows] = { "c1", "C2" };

	int rowDebInds[2] = { 0, 2 };
	int nbCoefPerRow[2] = { 2, 2 };
	int colInds[4] = { 0, 1, 0, 1 };
	double coefs[4] = { 3., 1.0, 0.3, 3. };

	SRScreatecols(srsProb, nbCols, obj, colTypesMip, lbMip, ubMip, colNames);
	SRScreaterows(srsProb, nbRows, rhsMip, range, sense, rowNames);
	SRSsetcoefs(srsProb, rowDebInds, nbCoefPerRow, colInds, coefs);
	//SRSwritempsprob(srsProb->problem_mps, "problemMip.mps");

	SRSoptimize(srsProb);

	{
		double objVal = -1.0;
		int nbIter = -1;
		SRSgetobjval(srsProb, &objVal);
		SRSgetmipitercount(srsProb, &nbIter);
		printf("Objective value : %lf, nb iter mip %d\n", objVal, nbIter);
	}

	SRSfreeprob(srsProb);
	
	exit(0);

	// *** read MPS MIP ***
	{
		srsProb = SRScreateprob();
		SRSreadmpsprob(srsProb, "pb_topase_long_A_JEU_DE_DONNEES");
		//SRSreadmpsprob(srsProb, "Donnees_Probleme_Solveur.mps");

		SRSoptimize(srsProb);

		double objVal = -1.0;
		int nbIter = -1;
		SRSgetobjval(srsProb, &objVal);
		SRSgetmipitercount(srsProb, &nbIter);
		printf("Objective value : %lf, nb iter mip %d\n", objVal, nbIter);

		SRSfreeprob(srsProb);
	}
	return 0;
}