// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

#include "librairie_pne.h"

# include "pne_sys.h"
# include <stdio.h>
# include <ctype.h>

# ifdef __cplusplus
# include "pne_definition_arguments.h"
# include "pne_constantes_externes.h"	       						 
# include "pne_define.h"
extern "C"
{
	void PNE_Solveur(PROBLEME_A_RESOUDRE *);
	void PNE_LireJeuDeDonneesMPS(void);
}
# else
#include "pne_fonctions.h"
#include "pne_constantes_externes.h"	       						 
#include "pne_define.h"

#include "spx_define.h"
#include "spx_constantes_externes.h"
#include "spx_fonctions.h"

# endif

# include "mps_define.h"
# include "pne_definition_arguments.h"
//# include "mps_extern_global.h"

# include "bb_define.h"

#include <xprs.h>
//#include <coin/ClpSimplex.hpp>
//#include <coin/ClpSolve.hpp>
#include <coin/Clp_C_Interface.h>




void PNEnewProb(PNE_PROB*ptr ){
	*ptr = (void*)malloc(sizeof(PROBLEME_A_RESOUDRE));
}


int PNEncols(PNE_PROB arg) {
	PROBLEME_A_RESOUDRE * prob = (PROBLEME_A_RESOUDRE*)arg;
	return prob->NombreDeVariables;
}
int PNEnrows(PNE_PROB arg) {
	PROBLEME_A_RESOUDRE * prob = (PROBLEME_A_RESOUDRE*)arg;
	return prob->NombreDeContraintes;
}
int PNEneles(PNE_PROB arg) {
	PROBLEME_A_RESOUDRE * prob = (PROBLEME_A_RESOUDRE*)arg;
	return *prob->NombreDeTermesDesLignes;
}

void PNEreadmps(PNE_PROB arg, char const * file_name) {
	PROBLEME_A_RESOUDRE * prob = (PROBLEME_A_RESOUDRE*)arg;
	int ToleranceDOptimaliteExterne = 0;
	double ToleranceExterne = 0.01;

	//PROBLEME_MPS * Mps = malloc(sizeof(PROBLEME_MPS));
	PROBLEME_MPS Mps;
	printf(" --- begin MPS read ---\n");
	PNE_LireJeuDeDonneesMPS_AvecNom(&Mps, file_name);

	// create PROBLEME_MPS
	printf(" --- MPS read ---\n");
	printf(" Probleme is %p\n", prob);

	PNE_copy_problem(&Mps, prob, ToleranceDOptimaliteExterne, ToleranceExterne);
}

int SHARED_EXPORT PNEsolve(PNE_PROB arg) {
	PROBLEME_A_RESOUDRE * prob = (PROBLEME_A_RESOUDRE*)arg;
	int YaUneSolution;
	//Probleme->FaireDuPresolve = NON_PNE;
	PNE_Solveur(prob);

	YaUneSolution = prob->ExistenceDUneSolution;
	//PNE_LireJeuDeDonneesMPS_AvecNom(prob, file_name);
	return YaUneSolution;
}
void errormsg(XPRSprob prob, const char *sSubName, int nLineNo, int nErrCode);
void XPRS_CC optimizermsg(XPRSprob prob, void* data, const char *sMsg, int nLen, int nMsglvl);



void launch_xpress(PNE_PROB arg) {
	PROBLEME_A_RESOUDRE * prob = (PROBLEME_A_RESOUDRE*)arg;
	printf("----- launch_xpress -----\n");
	int     NombreDeVariables = prob->NombreDeVariables;
	int *   TypeDeVariable = prob->TypeDeVariable;
	int *   TypeDeBorne = prob->TypeDeBorneDeLaVariable;
	double * Xmax = prob->Xmax;
	double * Xmin = prob->Xmin;
	double * CoutLineaire = prob->CoutLineaire;
	int     NombreDeContraintes = prob->NombreDeContraintes;
	char * Sens = prob->Sens;
	double * SecondMembre = prob->SecondMembre;
	int *   Mdeb = prob->IndicesDebutDeLigne;
	int *   Nbterm = prob->NombreDeTermesDesLignes;
	int *   IndicesColonnes = prob->IndicesColonnes;
	double * CoefficientsDeLaMatriceDesContraintes = prob->CoefficientsDeLaMatriceDesContraintes;

	int nReturn;
	XPRSprob xprs_prob = NULL;
	nReturn = XPRSinit("");
	if (nReturn != 0)errormsg(xprs_prob, "XPRSinit", __LINE__, nReturn);
	nReturn = XPRScreateprob(&xprs_prob);
	if (nReturn != 0)errormsg(xprs_prob, "XPRScreateprob", __LINE__, nReturn);

	XPRSsetcbmessage(xprs_prob, optimizermsg, NULL);
	if (nReturn != 0)errormsg(xprs_prob, "XPRSsetcbmessage", __LINE__, nReturn);
	// create empty problem
	nReturn = XPRSloadlp(xprs_prob, "test", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if (nReturn != 0)errormsg(xprs_prob, "XPRSloadlp", __LINE__, nReturn);
	// add variables
	char * row_type = malloc(NombreDeContraintes * sizeof(char));

	int    * row_beg = malloc((1 + NombreDeContraintes) * sizeof(int));

	char   * type_mip = NULL;
	int    * col_mip = NULL;

	int nz = 0;
	//
	int nInteger = 0;
	for (int i = 0; i < NombreDeVariables; ++i) {
		if (TypeDeVariable[i] == ENTIER)
			++nInteger;
	}
	for (int i = 0; i < NombreDeContraintes; ++i) {
		if (Sens[i] == '>')
			row_type[i] = 'G';
		else if (Sens[i] == '<')
			row_type[i] = 'L';
		else
			row_type[i] = 'E';
		row_beg[i] = nz;
		nz += Nbterm[i];
	}
	row_beg[NombreDeContraintes + 1] = nz;

	nReturn = XPRSaddcols(xprs_prob, NombreDeVariables, 0, CoutLineaire, 0, 0, 0, Xmin, Xmax);
	if (nReturn != 0)errormsg(xprs_prob, "XPRSaddcols", __LINE__, nReturn);

	nReturn = XPRSaddrows(xprs_prob, NombreDeContraintes, nz, row_type, SecondMembre, 0, row_beg, IndicesColonnes, CoefficientsDeLaMatriceDesContraintes);
	if (nReturn != 0)errormsg(xprs_prob, "XPRSaddrows", __LINE__, nReturn);

	free(row_type);
	free(row_beg);

	XPRSsetintcontrol(xprs_prob, XPRS_THREADS, 1);
	XPRSsetintcontrol(xprs_prob, XPRS_DEFAULTALG, XPRS_ALG_DUAL);

	if (nInteger > 0) {
		col_mip = malloc(nInteger * sizeof(int));
		type_mip = malloc(nInteger * sizeof(char));
		int n = 0;
		for (int i = 0; i < NombreDeVariables; ++i) {
			if (TypeDeVariable[i] == ENTIER) {
				col_mip[n] = i;
				type_mip[n] = 'B';
				++n;
			}
		}
		XPRSchgcoltype(xprs_prob, nInteger, col_mip, type_mip);
		free(col_mip);
		free(type_mip);
		XPRSmipoptimize(xprs_prob, "");
		XPRSgetmipsol(xprs_prob, prob->X, NULL);
	}
	else {
		XPRSlpoptimize(xprs_prob, "");
		XPRSgetlpsol(xprs_prob, prob->X, NULL, prob->VariablesDualesDesContraintes, NULL);
	}
	prob->ExistenceDUneSolution = SOLUTION_OPTIMALE_TROUVEE;
	XPRSdestroyprob(xprs_prob);
	XPRSfree();
	printf("----- finish_xpress -----\n");
}


void launch_clp(PNE_PROB arg) {
	PROBLEME_A_RESOUDRE * prob = (PROBLEME_A_RESOUDRE*)arg;
	printf("----- launch_clp-----\n");
	int     NombreDeVariables = prob->NombreDeVariables;
	int *   TypeDeVariable = prob->TypeDeVariable;
	int *   TypeDeBorne = prob->TypeDeBorneDeLaVariable;
	double * Xmax = prob->Xmax;
	double * Xmin = prob->Xmin;
	double * CoutLineaire = prob->CoutLineaire;
	int     NombreDeContraintes = prob->NombreDeContraintes;
	char * Sens = prob->Sens;
	double * SecondMembre = prob->SecondMembre;
	int *   Mdeb = prob->IndicesDebutDeLigne;
	int *   Nbterm = prob->NombreDeTermesDesLignes;
	int *   IndicesColonnes = prob->IndicesColonnes;
	double * CoefficientsDeLaMatriceDesContraintes = prob->CoefficientsDeLaMatriceDesContraintes;

	int    * row_beg = malloc((1 + NombreDeContraintes) * sizeof(int));
	double * cMin = malloc(NombreDeContraintes * sizeof(double));
	double * cMax = malloc(NombreDeContraintes * sizeof(double));

	int nz = 0;
	int fake_nz = 0;
	int real_nz = 0;
	int * real_nz_IndicesColonnes = NULL;
	double * real_nz_CoefficientsDeLaMatriceDesContraintes = NULL;
	int old_row = -1;
	Clp_Simplex * clp = Clp_newModel();
	
	Clp_loadProblem(clp, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	
	Clp_addColumns(clp, NombreDeVariables, Xmin, Xmax, CoutLineaire, 0, 0, 0);
	//printf("Clp_getNumCols() is %d\n", Clp_getNumCols(clp));
	//printf("Clp_getNumRows() is %d\n", Clp_getNumRows(clp));
	
	nz = 0;
	for (int i = 0; i < NombreDeContraintes; ++i) {
		if (Sens[i] == '>') {
			cMin[i] = SecondMembre[i];
			cMax[i] = Clp_getInfinity(clp);
		}
		else if (Sens[i] == '<') {
			cMin[i] = -Clp_getInfinity(clp);
			cMax[i] = SecondMembre[i];
		}
		else {
			cMin[i] = SecondMembre[i];
			cMax[i] = SecondMembre[i];
		}
		nz += Nbterm[i];
	}
	// recompute the matrix without 0 coefficient
	real_nz_IndicesColonnes = malloc(nz * sizeof(int));
	real_nz_CoefficientsDeLaMatriceDesContraintes = malloc(nz * sizeof(double));
	real_nz = 0;
	int id = 0;
	for (int i = 0; i < NombreDeContraintes; ++i) {
		row_beg[i] = real_nz;
		for (int j = 0; j < Nbterm[i]; ++j, ++id) {
			if (fabs(CoefficientsDeLaMatriceDesContraintes[id]) == 0) {
				++fake_nz;
			}
			else {
				real_nz_CoefficientsDeLaMatriceDesContraintes[real_nz] = CoefficientsDeLaMatriceDesContraintes[id];
				real_nz_IndicesColonnes[real_nz] = IndicesColonnes[id];
				++real_nz;
			}
		}
	}
	row_beg[NombreDeContraintes] = real_nz;
	
	Clp_addRows(clp, NombreDeContraintes, cMin, cMax, row_beg, real_nz_IndicesColonnes, real_nz_CoefficientsDeLaMatriceDesContraintes);
	
	Clp_initialSolve(clp);

	double const * x = Clp_getColSolution(clp);
	for (int i = 0; i < NombreDeVariables; ++i) {
		prob->X[i] = x[i];
	}
	double const * dual = Clp_getRowPrice(clp);
	for (int i = 0; i < NombreDeContraintes; ++i) {
		prob->VariablesDualesDesContraintes[i] = dual[i];
	}
	prob->ExistenceDUneSolution = SOLUTION_OPTIMALE_TROUVEE;

	free(cMin);
	free(cMax);
	free(row_beg);
	free(real_nz_IndicesColonnes);
	free(real_nz_CoefficientsDeLaMatriceDesContraintes);
	Clp_deleteModel(clp);	
	printf("----- finish_clp -----\n");
}


/************************************************************************************\
* Name:         optimizermsg                                                              *
* Purpose:      Display Optimizer error messages and warnings.                            *
* Arguments:    const char *sMsg    Message string                                   *
*               int nLen            Message length                                   *
*               int nMsgLvl         Message type                                     *
* Return Value: None.                                                                *
\************************************************************************************/

void XPRS_CC optimizermsg(XPRSprob prob, void* data, const char *sMsg, int nLen, int nMsglvl)
{
	switch (nMsglvl) {

		/* Print Optimizer error messages and warnings */
	case 4:       /* error */
	case 3:       /* warning */
	case 2:       /* dialogue */
	case 1:       /* information */
		printf("%*s\n", nLen, sMsg);
		break;

		/* Exit and flush buffers */
	default:
		fflush(NULL);
		break;
	}
}


/************************************************************************************\
* Name:         errormsg                                                             *
* Purpose:      Display error information about failed subroutines.                  *
* Arguments:    const char *sSubName    Subroutine name                              *
*               int nLineNo             Line number                                  *
*               int nErrCode            Error code                                   *
* Return Value: None                                                                 *
\************************************************************************************/
void errormsg(XPRSprob prob, const char *sSubName, int nLineNo, int nErrCode)
{
	int nErrNo;      /* Optimizer error number */

					 /* Print error message */
	printf("The subroutine %s has failed on line %d\n", sSubName, nLineNo);

	/* Append the error code if it exists */
	if (nErrCode != -1) printf("with error code %d.\n\n", nErrCode);

	/* Append Optimizer error number, if available */
	if (nErrCode == 32) {
		XPRSgetintattrib(prob, XPRS_ERRORCODE, &nErrNo);
		printf("The Optimizer eror number is: %d\n\n", nErrNo);
	}

	/* Free memory close files and exit */
	XPRSdestroyprob(prob);
	XPRSfree();
	exit(nErrCode);
}



void SHARED_EXPORT launch_spx_clp(PNE_PROB  arg) {
	PROBLEME_SIMPLEXE * prob = (PROBLEME_SIMPLEXE*)arg;
	printf("----- launch_clp-----\n");
	int     NombreDeVariables = prob->NombreDeVariables;
	int *   TypeDeVariable = prob->TypeDeVariable;
	double * Xmax = prob->Xmax;
	double * Xmin = prob->Xmin;
	double * CoutLineaire = prob->CoutLineaire;
	int     NombreDeContraintes = prob->NombreDeContraintes;
	char * Sens = prob->Sens;
	double * SecondMembre = prob->SecondMembre;
	int *   Mdeb = prob->IndicesDebutDeLigne;
	int *   Nbterm = prob->NombreDeTermesDesLignes;
	int *   IndicesColonnes = prob->IndicesColonnes;
	double * CoefficientsDeLaMatriceDesContraintes = prob->CoefficientsDeLaMatriceDesContraintes;

	int    * row_beg = malloc((1 + NombreDeContraintes) * sizeof(int));
	double * cMin = malloc(NombreDeContraintes * sizeof(double));
	double * cMax = malloc(NombreDeContraintes * sizeof(double));

	int nz = 0;
	int fake_nz = 0;
	int real_nz = 0;
	int * real_nz_IndicesColonnes = NULL;
	double * real_nz_CoefficientsDeLaMatriceDesContraintes = NULL;
	int old_row = -1;

	//printf("Clp_newModel\n");

	Clp_Simplex * clp = Clp_newModel();

	//printf("NombreDeVariables   is %10d\n", NombreDeVariables);
	//printf("NombreDeContraintes is %10d\n", NombreDeContraintes);

	//printf("Clp_loadProblem\n");
	Clp_loadProblem(clp, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	//printf("Clp_addColumns\n");
	Clp_addColumns(clp, NombreDeVariables, Xmin, Xmax, CoutLineaire, 0, 0, 0);
	
	//printf("Clp_getNumCols() is %d\n", Clp_getNumCols(clp));
	//printf("Clp_getNumRows() is %d\n", Clp_getNumRows(clp));
	//printf("Sense\n");
	nz = 0;
	for (int i = 0; i < NombreDeContraintes; ++i) {
		if (Sens[i] == '>') {
			cMin[i] = SecondMembre[i];
			cMax[i] = Clp_getInfinity(clp);
		}
		else if (Sens[i] == '<') {
			cMin[i] = -Clp_getInfinity(clp);
			cMax[i] = SecondMembre[i];
		}
		else {
			cMin[i] = SecondMembre[i];
			cMax[i] = SecondMembre[i];
		}
		nz += Nbterm[i];
	}
	//printf("recompute the matrix without 0 coefficient\n");
	// recompute the matrix without 0 coefficient
	real_nz_IndicesColonnes = malloc(nz * sizeof(int));
	real_nz_CoefficientsDeLaMatriceDesContraintes = malloc(nz * sizeof(double));
	real_nz = 0;
	int id = 0;
	for (int i = 0; i < NombreDeContraintes; ++i) {
		row_beg[i] = real_nz;
		for (int j = 0; j < Nbterm[i]; ++j, ++id) {
			if (fabs(CoefficientsDeLaMatriceDesContraintes[id]) == 0) {
				++fake_nz;
			}
			else {
				real_nz_CoefficientsDeLaMatriceDesContraintes[real_nz] = CoefficientsDeLaMatriceDesContraintes[id];
				real_nz_IndicesColonnes[real_nz] = IndicesColonnes[id];
				++real_nz;
			}
		}
	}
	row_beg[NombreDeContraintes] = real_nz;

	//printf("Clp_addRows\n");
	Clp_addRows(clp, NombreDeContraintes, cMin, cMax, row_beg, real_nz_IndicesColonnes, real_nz_CoefficientsDeLaMatriceDesContraintes);

	//printf("Clp_initialSolve\n");
	Clp_initialSolve(clp);

	double const * x = Clp_getColSolution(clp);
	for (int i = 0; i < NombreDeVariables; ++i) {
		prob->X[i] = x[i];
	}
	double const * dual = Clp_getRowPrice(clp);
	for (int i = 0; i < NombreDeContraintes; ++i) {
		prob->CoutsMarginauxDesContraintes[i] = dual[i];
	}
	prob->ExistenceDUneSolution = SOLUTION_OPTIMALE_TROUVEE;

	free(cMin);
	free(cMax);
	free(row_beg);
	free(real_nz_IndicesColonnes);
	free(real_nz_CoefficientsDeLaMatriceDesContraintes);
	Clp_deleteModel(clp);
	printf("----- finish_clp -----\n");
}