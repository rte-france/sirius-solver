// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sirius_callback.h"
#include "srs_api.h"
#include "pne_constantes_externes.h"
#include "pne_fonctions.h"
#include "spx_fonctions.h"

#ifndef difftimens
#define difftimens(ts2, ts1) (double)ts2.tv_sec - (double)ts1.tv_sec + ((double)ts2.tv_nsec - (double)ts1.tv_nsec)/1.e9
#endif

# ifdef __cplusplus
extern "C"
{
# endif

void PNE_copy_problem(PROBLEME_MPS * Mps, PROBLEME_A_RESOUDRE * Probleme, int ToleranceDOptimaliteExterne, double ToleranceExterne) {
	Probleme->NombreDeVariables = Mps->NbVar;
	Probleme->NomsDesVariables = Mps->LabelDeLaVariable;
	Probleme->TypeDeVariable = Mps->TypeDeVariable;
	Probleme->TypeDeBorneDeLaVariable = Mps->TypeDeBorneDeLaVariable;
	Probleme->X = Mps->U;
	Probleme->Xmax = Mps->Umax;
	Probleme->Xmin = Mps->Umin;
	Probleme->CoutLineaire = Mps->L;
	Probleme->NombreDeContraintes = Mps->NbCnt;
	Probleme->NomsDesContraintes = Mps->LabelDeLaContrainte;
	Probleme->SecondMembre = Mps->B;
	Probleme->Sens = Mps->SensDeLaContrainte;
	Probleme->IndicesDebutDeLigne = Mps->Mdeb;
	Probleme->NombreDeTermesDesLignes = Mps->NbTerm;
	Probleme->CoefficientsDeLaMatriceDesContraintes = Mps->A;
	Probleme->IndicesColonnes = Mps->Nuvar;
	Probleme->VariablesDualesDesContraintes = Mps->VariablesDualesDesContraintes;
	Probleme->SortirLesDonneesDuProbleme = NON_PNE;
	Probleme->AlgorithmeDeResolution = SIMPLEXE; /* SIMPLEXE ou POINT_INTERIEUR */
	Probleme->CoupesLiftAndProject = NON_PNE;
	Probleme->AffichageDesTraces = OUI_PNE;
	Probleme->FaireDuPresolve = OUI_PNE /* OUI_PNE */;
    Probleme->objective_offset = Mps->objective_offset;
	if (Probleme->FaireDuPresolve == NON_PNE) printf("!!!!!!!!!!!!!!  Attention pas de presolve   !!!!!!!!!\n");

	Probleme->TempsDExecutionMaximum = 0;
	Probleme->NombreMaxDeSolutionsEntieres = -1;
	Probleme->ToleranceDOptimalite = 1.e-4; /* C'est en % donc 1.e-4 ca fait 1.e-6 */

	if (ToleranceDOptimaliteExterne == 1) {
		Probleme->ToleranceDOptimalite = ToleranceExterne;
		printf("gap for optimality was given by the user: %f (must be postive or zero)\n", ToleranceExterne);
	}
}

const char * SRSversion() {
	return "SIRIUS V1.0.0";
}

// *** Start of Utility functions ***
int mallocAndCopyIntArray(size_t arraySize, const int * sourceArray, int ** targetArrayPointer) {
	size_t arraySizeInBytes = arraySize * sizeof(int);
	(*targetArrayPointer) = malloc(arraySizeInBytes);
	memcpy((*targetArrayPointer), sourceArray, arraySizeInBytes);
	return 0;
}

int mallocAndCopyDoubleArray(size_t arraySize, const double * sourceArray, double ** targetArrayPointer) {
	size_t arraySizeInBytes = arraySize * sizeof(double);
	(*targetArrayPointer) = malloc(arraySizeInBytes);
	memcpy((*targetArrayPointer), sourceArray, arraySizeInBytes);
	return 0;
}

int mallocAndCopyCharArray(size_t arraySize, const char * sourceArray, char ** targetArrayPointer) {
	size_t arraySizeInBytes = arraySize * sizeof(char);
	(*targetArrayPointer) = malloc(arraySizeInBytes);
	memcpy((*targetArrayPointer), sourceArray, arraySizeInBytes);
	return 0;
}

int mallocAndCopyString(const char * sourceString, char ** targetString) {
	if (sourceString == NULL) {
		(*targetString) = NULL;
		return 0;
	}
	size_t stringSize = strlen(sourceString) + 1;
	size_t stringSizeInBytes = stringSize * sizeof(char);
	(*targetString) = malloc(stringSizeInBytes);
#ifdef __unix__
	strcpy((*targetString), sourceString);
#else
	strcpy_s((*targetString), stringSizeInBytes, sourceString);
#endif
	return 0;
}

int mallocAndCopyStringArray(size_t arraySize, char const *const * sourceArray, char * ** targetArrayPointer) {
	size_t arraySizeInBytes = arraySize * sizeof(char *);
	(*targetArrayPointer) = malloc(arraySizeInBytes);
	for (int idxString = 0; idxString < arraySize; ++idxString) {
		mallocAndCopyString(sourceArray[idxString], &(*targetArrayPointer)[idxString]);
	}
	return 0;
}

int computeColBoundType(double lb, double ub) {
	if (lb > -SRS_infinite) {
		if (ub < SRS_infinite) {
			return VARIABLE_BORNEE_DES_DEUX_COTES;
		}
		return VARIABLE_BORNEE_INFERIEUREMENT;
	}
	else if (ub < SRS_infinite) {
		return VARIABLE_BORNEE_SUPERIEUREMENT;
	}
	return VARIABLE_NON_BORNEE;
}
// *** Enf of Utility functions ***

int initProblemMpsPointer(SRS_PROBLEM * problem_srs) {
    PROBLEME_MPS* mps = (PROBLEME_MPS *)malloc(sizeof(PROBLEME_MPS));
	mps->LabelDeLObjectif = NULL;
	mps->NbVar = 0;
	mps->NbCnt = 0;
	mps->Msui = NULL;
	mps->Mder = NULL;
	mps->Nuvar = NULL;
	mps->BRange = NULL;
	mps->LabelDeLaContrainte = NULL;
	mps->LabelDuSecondMembre = NULL;
	mps->A = NULL;
	mps->Mdeb = NULL;
	mps->NbTerm = NULL;
	mps->B = NULL;
	mps->SensDeLaContrainte = NULL;
	mps->VariablesDualesDesContraintes = NULL;
	mps->TypeDeVariable = NULL;
	mps->TypeDeBorneDeLaVariable = NULL;
	mps->U = NULL;
	mps->L = NULL;
	mps->Umin = NULL;
	mps->Umax = NULL;
	mps->LabelDeLaVariable = NULL;
    mps->objective_offset = 0;

	problem_srs->problem_mps = mps;
	return 0;
}

int SRSdefault_callback(void *caller, const char *sMsg, int nLen, SIRIUS_LOGLEVEL log_level)
{
	if (nLen <= 0)
	{
		return 0;
	}
	if (log_level == SIRIUS_ERROR)
	{
		fprintf(stderr, sMsg);
	}
	else
	{
		printf("%s", sMsg);
	}
}

SRS_PROBLEM * SRScreateprob() {
	SRS_PROBLEM * problem_srs = (SRS_PROBLEM *)malloc(sizeof(SRS_PROBLEM));
	
	problem_srs->problem_mip = NULL;
	problem_srs->problem_simplexe = NULL;
	problem_srs->problem_spx = NULL;
	problem_srs->read_an_mps = false;
	problem_srs->is_mip = false;
	problem_srs->maximize = false;
	//params
	problem_srs->verboseSpx = 0;
	problem_srs->verbosePne = 0;
	problem_srs->relativeGap = 1e-4;
	problem_srs->presolve = 1;
	problem_srs->scaling = 1;
	problem_srs->maxTime = 0.;
	
	//MPS
	initProblemMpsPointer(problem_srs);
	problem_srs->callback = SRSdefault_callback;

	return problem_srs;
}

int SRSfreempsprob(PROBLEME_MPS * problem_mps) {
	//free a bunch of stuff
	if (problem_mps != NULL) {

		// unique pointer
		free(problem_mps->LabelDeLObjectif);

		// ~ nb terms
		free(problem_mps->A);
		free(problem_mps->Nuvar);
		free(problem_mps->Msui);
		
		// nb rows
		free(problem_mps->Mdeb);
		free(problem_mps->NbTerm);
		free(problem_mps->Mder);
		free(problem_mps->B);
		free(problem_mps->SensDeLaContrainte);
		free(problem_mps->BRange);
		free(problem_mps->VariablesDualesDesContraintes);
		for (int idxRow = 0; idxRow < problem_mps->NbCnt; ++idxRow) {
			if (problem_mps->LabelDeLaContrainte != NULL)
				free(problem_mps->LabelDeLaContrainte[idxRow]);
			if (problem_mps->LabelDuSecondMembre != NULL)
				free(problem_mps->LabelDuSecondMembre[idxRow]);
		}
		free(problem_mps->LabelDeLaContrainte);
		free(problem_mps->LabelDuSecondMembre);

		// nb cols
		free(problem_mps->TypeDeVariable);
		free(problem_mps->TypeDeBorneDeLaVariable);
		free(problem_mps->U);
		free(problem_mps->L);
		free(problem_mps->Umin);
		free(problem_mps->Umax);
		if (problem_mps->LabelDeLaVariable != NULL) {
			for (int idxCol = 0; idxCol < problem_mps->NbVar; ++idxCol) {
				free(problem_mps->LabelDeLaVariable[idxCol]);
			}
			free(problem_mps->LabelDeLaVariable);
		}

		free(problem_mps);

	}

	return 0;
}

int SRSwritempsprob(PROBLEME_MPS * problem_mps, const char * fileName) {
	PROBLEME_A_RESOUDRE problem_a_resoudre;
	PROBLEME_PNE problem_pne;
	PNE_copy_problem(problem_mps, &problem_a_resoudre, 0, 0.0);
	PNE_EcrireJeuDeDonneesMPS_avecNom(&problem_pne, &problem_a_resoudre, fileName);

	return 0;
}

int SRSreadmpsprob(SRS_PROBLEM * problem_srs, const char * fileName) {
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;
	if (problem_srs->read_an_mps) {
		SRSfreempsprob(problem_mps);
		initProblemMpsPointer(problem_srs);
	}
	
	//PNE_LireJeuDeDonneesMPS_AvecNom(problem_srs->problem_mps, fileName);

	for (int idxCol = 0; idxCol < problem_srs->problem_mps->NbVar; ++idxCol) {
		if (problem_srs->problem_mps->TypeDeVariable[idxCol] != REEL) {
			problem_srs->is_mip = true;
			break;
		}
	}

	problem_srs->read_an_mps = true;

	return 0;
}

int SRSfreeprob(SRS_PROBLEM * problem_srs) {
	//free a bunch of stuff
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;
	if (problem_mps != NULL) {
		SRSfreempsprob(problem_mps);
	}

	if (problem_srs->problem_spx != NULL) {
		SPX_LibererProbleme(problem_srs->problem_spx);
	}

	if (problem_srs->problem_simplexe != NULL) {
		free(problem_srs->problem_simplexe->ComplementDeLaBase);
		free(problem_srs->problem_simplexe->PositionDeLaVariable);
		free(problem_srs->problem_simplexe->CoutsReduits);
		free(problem_srs->problem_simplexe);
	}

	if (problem_srs->problem_mip != NULL) {
		free(problem_srs->problem_mip);
	}

	free(problem_srs);


	return 0;
}

int SRScreatecols(SRS_PROBLEM * problem_srs,
	int nb_cols, const double * obj_coefs, const int * col_types, const double * lb, const double * ub, char const *const * col_names
) {
	if (problem_srs->read_an_mps) {
		fprintf(stderr, "(ERROR) Can't create cols if an mps file has been read\n");
		return -1;
	}
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;

	problem_mps->NbVar = nb_cols;
	problem_mps->U = malloc(nb_cols * sizeof(double));

	mallocAndCopyIntArray(nb_cols, col_types, &problem_mps->TypeDeVariable);
	mallocAndCopyDoubleArray(nb_cols, obj_coefs, &problem_mps->L);
	mallocAndCopyDoubleArray(nb_cols, lb, &problem_mps->Umin);
	mallocAndCopyDoubleArray(nb_cols, ub, &problem_mps->Umax);
	
	problem_mps->TypeDeBorneDeLaVariable = malloc(nb_cols * sizeof(int));
	for (int idxCol = 0; idxCol < nb_cols; ++idxCol) {
		problem_mps->TypeDeBorneDeLaVariable[idxCol] = computeColBoundType(lb[idxCol], ub[idxCol]);
		if ((!problem_srs->is_mip) && (col_types[idxCol] != REEL)) {
			problem_srs->is_mip = true;
		}
	}

	if (col_names != NULL) {
		mallocAndCopyStringArray(nb_cols, col_names, &problem_mps->LabelDeLaVariable);
	}

	return 0;
}
int SRScreaterows(SRS_PROBLEM * problem_srs,
	int nb_rows, const double * rhs, const double * range, const char * sense, char const *const * row_names
){
	if (problem_srs->read_an_mps) {
		fprintf(stderr, "(ERROR) Can't create rows if an mps file has been read\n");
		return -1;
	}
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;

	problem_mps->NbCnt = nb_rows;
	problem_mps->VariablesDualesDesContraintes = malloc(nb_rows * sizeof(double));
	
	mallocAndCopyDoubleArray(nb_rows, rhs, &problem_mps->B);
	mallocAndCopyCharArray(nb_rows, sense, &problem_mps->SensDeLaContrainte);
	mallocAndCopyDoubleArray(nb_rows, range, &problem_mps->BRange);
	
	if (row_names != NULL) {
		mallocAndCopyStringArray(nb_rows, row_names, &problem_mps->LabelDeLaContrainte);
	}

	return 0;
}
int SRSsetcoefs(SRS_PROBLEM * problem_srs,
	int   * matBeg, // Pour chaque ligne, indice debut de la ligne dans le vecteur des coefficients
	int   * nbTermsPerRow, // Nombre de termes non nuls de la ligne
	int   * termsColIdx, // Vecteur parallele au suivant. Il contient l'indice colonne de chaque coefficient
	double * coefs // Coefficients de la matrice des contraintes
) {
	if (problem_srs->read_an_mps) {
		fprintf(stderr, "(ERROR) Can't set coefs if an mps file has been read\n");
		return -1;
	}
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;

	int nbRows = problem_mps->NbCnt;
	size_t nbTerms = 0;
	for (int idxRow = 0; idxRow < nbRows; ++idxRow) {
		nbTerms += nbTermsPerRow[idxRow];
	}

	mallocAndCopyDoubleArray(nbTerms, coefs, &problem_mps->A);
	mallocAndCopyIntArray(nbTerms, termsColIdx, &problem_mps->Nuvar);
	mallocAndCopyIntArray(nbRows, matBeg, &problem_mps->Mdeb);
	mallocAndCopyIntArray(nbRows, nbTermsPerRow, &problem_mps->NbTerm);

	return 0;
}

int allocateProblemsAndPropagateParams(SRS_PROBLEM * problem_srs) {
	if (problem_srs->is_mip) {
		problem_srs->problem_mip = malloc(sizeof(PROBLEME_A_RESOUDRE));
		PNE_copy_problem(problem_srs->problem_mps, problem_srs->problem_mip, 0, 0.0);
		
		problem_srs->problem_mip->FaireDuPresolve = problem_srs->presolve;
		problem_srs->problem_mip->ToleranceDOptimalite = problem_srs->relativeGap;
		problem_srs->problem_mip->TempsDExecutionMaximum = (int)problem_srs->maxTime;
		problem_srs->problem_mip->AffichageDesTraces = problem_srs->verbosePne;
		//problem_srs->problem_mip->FaireDuScaling = problem_srs->scaling;

		problem_srs->problem_mip->TempsDExecutionMaximum = (int)problem_srs->maxTime;

	} else {
		PROBLEME_MPS * problem_mps = problem_srs->problem_mps;
		PROBLEME_SIMPLEXE * problem_simplexe = problem_srs->problem_simplexe;
		PROBLEME_SPX * problem_spx = problem_srs->problem_spx;

		if (problem_simplexe == NULL) {
			problem_simplexe = problem_srs->problem_simplexe = malloc(sizeof(PROBLEME_SIMPLEXE));
			problem_simplexe->ComplementDeLaBase = (int*)malloc(problem_mps->NbCnt * sizeof(int));
			problem_simplexe->PositionDeLaVariable = (int*)malloc(problem_mps->NbVar * sizeof(int));
			problem_simplexe->CoutsReduits = (double*)malloc(problem_mps->NbVar * sizeof(double));
		}
		SPXcopy_problem(problem_mps, problem_simplexe);

		if (problem_spx != NULL)
		{
			SPX_ModifierLeVecteurCouts(problem_spx, problem_mps->L, problem_mps->NbVar);
			SPX_ModifierLeVecteurSecondMembre(problem_spx, problem_mps->B, problem_mps->SensDeLaContrainte, problem_mps->NbCnt);
			
			size_t arraySizeInBytes = problem_mps->NbVar * sizeof(double);
			memcpy(problem_spx->Xmin, problem_mps->Umin, arraySizeInBytes);
			memcpy(problem_spx->Xmax, problem_mps->Umax, arraySizeInBytes);

			memcpy(problem_simplexe->Xmin, problem_mps->Umin, arraySizeInBytes);
			memcpy(problem_simplexe->Xmax, problem_mps->Umax, arraySizeInBytes);

			problem_simplexe->Contexte = BRANCH_AND_BOUND_OU_CUT_NOEUD;
			problem_simplexe->BaseDeDepartFournie = UTILISER_LA_BASE_DU_PROBLEME_SPX;
		}
		else
		{
			problem_simplexe->Contexte = SIMPLEXE_SEUL;
			problem_simplexe->BaseDeDepartFournie = NON_SPX;

			problem_simplexe->ChoixDeLAlgorithme = SPX_DUAL;

			problem_simplexe->TypeDePricing = PRICING_STEEPEST_EDGE;
			problem_simplexe->FaireDuScaling = OUI_SPX;
			problem_simplexe->StrategieAntiDegenerescence = AGRESSIF;

			problem_simplexe->LibererMemoireALaFin = NON_SPX;

			problem_simplexe->UtiliserCoutMax = NON_SPX;
			problem_simplexe->CoutMax = 0.0;
			problem_simplexe->NombreMaxDIterations = -1;
			problem_simplexe->DureeMaxDuCalcul = -1.;
			problem_simplexe->NombreDeContraintesCoupes = 0;
		}
		problem_simplexe->AffichageDesTraces = problem_srs->verboseSpx;
		problem_simplexe->FaireDuScaling = problem_srs->scaling;

		problem_simplexe->CoutMax = 0;
		problem_simplexe->NbVarDeBaseComplementaires = 0;
	}

	return 0;
}


int SRSoptimize(SRS_PROBLEM * problem_srs) {
	
	int nbCols = problem_srs->problem_mps->NbVar;
	if (problem_srs->maximize) {
		for (int idxCol = 0; idxCol < nbCols; ++idxCol) {
			problem_srs->problem_mps->L[idxCol] *= -1.0;
		}
	}
	
	allocateProblemsAndPropagateParams(problem_srs);

	struct timespec debut;
	timespec_get(&debut, TIME_UTC);
	if (problem_srs->is_mip) {
		// Appel de pne
		PNE_Solveur(problem_srs->problem_mip);
	} else {
		// Appel du simplexe
		problem_srs->problem_spx = SPX_Simplexe(problem_srs->problem_simplexe, problem_srs->problem_spx);
		//free(spxParams);
	}
	struct timespec fin;
	timespec_get(&fin, TIME_UTC);
	double TempsEcoule = difftimens(fin, debut);

	if (problem_srs->maximize < 0) {
		for (int idxCol = 0; idxCol < nbCols; ++idxCol) {
			problem_srs->problem_mps->L[idxCol] *= -1.0;
		}
	}


	return 0;
}

int SRSgetobjval(SRS_PROBLEM * problem_srs, double * objVal) {
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;
	(*objVal) = 0.0;
	int nbCols = problem_mps->NbVar;
	for (int idxCol = 0; idxCol < nbCols; ++idxCol) {
		(*objVal) += problem_mps->L[idxCol] * problem_mps->U[idxCol];
	}
	if (problem_srs->maximize)
		(*objVal) *= -1.;

    *objVal += problem_mps->objective_offset;

	return 0;
}

int SRSgetspxitercount(SRS_PROBLEM * problem_srs, int * iterCount) {
	if (problem_srs->problem_spx == NULL) {
		return -1;
	}
	(*iterCount) = problem_srs->problem_spx->Iteration;

	return 0;
}

int SRSgetmipitercount(SRS_PROBLEM * problem_srs, int * iterCount) {
	if (problem_srs->problem_mip == NULL) {
		return -1;
	}
	//FIXME (*iterCount) = problem_srs->problem_mip->SommeDuNombreDIterations;

	return 0;
}

int SPXcopy_problem(PROBLEME_MPS * problem_mps, PROBLEME_SIMPLEXE * problem_simplexe)
{
	problem_simplexe->NombreDeContraintesCoupes = 0;
	problem_simplexe->TypeDePricing = PRICING_STEEPEST_EDGE;//PRICING_STEEPEST_EDGE PRICING_DANTZIG()
	problem_simplexe->FaireDuScaling = OUI_SPX; // Vaut OUI_SPX ou NON_SPX
	problem_simplexe->StrategieAntiDegenerescence = AGRESSIF; // Vaut AGRESSIF ou PEU_AGRESSIF
	problem_simplexe->NombreMaxDIterations = -1; // si i < 0 , alors le simplexe prendre sa valeur par defaut
	problem_simplexe->DureeMaxDuCalcul = -1; // si i < 0 , alors le simplexe prendre sa valeur par defaut
	problem_simplexe->CoutLineaire = problem_mps->L;
	problem_simplexe->X = problem_mps->U;
	problem_simplexe->Xmin = problem_mps->Umin;
	problem_simplexe->Xmax = problem_mps->Umax;
	problem_simplexe->NombreDeVariables = problem_mps->NbVar;
	problem_simplexe->TypeDeVariable = problem_mps->TypeDeBorneDeLaVariable;
	problem_simplexe->NombreDeContraintes = problem_mps->NbCnt;
	problem_simplexe->IndicesDebutDeLigne = problem_mps->Mdeb;
	problem_simplexe->NombreDeTermesDesLignes = problem_mps->NbTerm;
	problem_simplexe->IndicesColonnes = problem_mps->Nuvar;
	problem_simplexe->CoefficientsDeLaMatriceDesContraintes = problem_mps->A;
	problem_simplexe->Sens = problem_mps->SensDeLaContrainte;
	problem_simplexe->SecondMembre = problem_mps->B;
	problem_simplexe->CoutsMarginauxDesContraintes = problem_mps->VariablesDualesDesContraintes;
	problem_simplexe->ChoixDeLAlgorithme = SPX_DUAL;

	problem_simplexe->LibererMemoireALaFin = NON_SPX;
	problem_simplexe->AffichageDesTraces = NON_SPX;
	problem_simplexe->CoutMax = -1;
	problem_simplexe->UtiliserCoutMax = NON_SPX;

	problem_simplexe->Contexte = SIMPLEXE_SEUL;
	problem_simplexe->BaseDeDepartFournie = NON_SPX;

	return 0;
}

int SRScopy_from_problem_simplexe(SRS_PROBLEM * problem_srs, PROBLEME_SIMPLEXE * problem_simplexe)
{
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;
	problem_mps->L = problem_simplexe->CoutLineaire;
	problem_mps->U = problem_simplexe->X;
	problem_mps->Umin = problem_simplexe->Xmin;
	problem_mps->Umax = problem_simplexe->Xmax;
	problem_mps->NbVar = problem_simplexe->NombreDeVariables;
	problem_mps->TypeDeBorneDeLaVariable = problem_simplexe->TypeDeVariable;
	problem_mps->NbCnt = problem_simplexe->NombreDeContraintes;
	problem_mps->Mdeb = problem_simplexe->IndicesDebutDeLigne;
	problem_mps->NbTerm = problem_simplexe->NombreDeTermesDesLignes;
	problem_mps->Nuvar = problem_simplexe->IndicesColonnes;
	problem_mps->A = problem_simplexe->CoefficientsDeLaMatriceDesContraintes;
	problem_mps->SensDeLaContrainte = problem_simplexe->Sens;
	problem_mps->B = problem_simplexe->SecondMembre;
	problem_mps->VariablesDualesDesContraintes = problem_simplexe->CoutsMarginauxDesContraintes;

	return 0;
}

int SRSchgrhs(SRS_PROBLEM * problem_srs, size_t nbRowIndexes, const int * rowIndexes, const double * newRhs) {
	for (int idx = 0; idx < nbRowIndexes; ++idx) {
		int rowAbsoluteIndex = rowIndexes[idx];
		problem_srs->problem_mps->B[rowAbsoluteIndex] = newRhs[idx];
	}

	return 0;
}

int SRSchgbds(SRS_PROBLEM * problem_srs, size_t nbColIndexes, const int * colIndexes, const double * newLb, const double * newUb) {
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;
	for (int relativeIndex = 0; relativeIndex < nbColIndexes; ++relativeIndex) {
		int absoluteIndex = colIndexes[relativeIndex];
		if (absoluteIndex > problem_mps->NbVar) {
			fprintf(stderr, "(ERROR) col %d does not exist, can't change its bounds\n", absoluteIndex);
			return -1;
		}
		problem_mps->Umin[absoluteIndex] = newLb[relativeIndex];
		problem_mps->Umax[absoluteIndex] = newUb[relativeIndex];
		problem_mps->TypeDeBorneDeLaVariable[absoluteIndex] = computeColBoundType(newLb[relativeIndex], newUb[relativeIndex]);
	}
	return 0;
}

int SRSchgobj(SRS_PROBLEM * problem_srs, size_t nbColIndexes, const int * colIndexes, const double * newObj) {
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;
	for (int relativeIndex = 0; relativeIndex < nbColIndexes; ++relativeIndex) {
		int absoluteIndex = colIndexes[relativeIndex];
		if (absoluteIndex > problem_mps->NbVar) {
			fprintf(stderr, "(ERROR) col %d does not exist, can't change its objective coef\n", absoluteIndex);
			return -1;
		}
		problem_mps->L[absoluteIndex] = newObj[relativeIndex];
	}
	return 0;
}

int SRSgetx(SRS_PROBLEM * problem_srs, double ** xValues) {
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;
	if ( (*xValues) == NULL ) {
		mallocAndCopyDoubleArray(problem_mps->NbVar, problem_mps->U, xValues);
	} else {
		size_t arraySizeInBytes = problem_mps->NbVar * sizeof(double);
		memcpy((*xValues), problem_mps->U, arraySizeInBytes);
	}
	return 0;
}

int SRSgetproblemstatus(SRS_PROBLEM * problem_srs) {
	if (SRSgetnbcols(problem_srs) == 0 && SRSgetnbrows(problem_srs) == 0) {
		return SRS_STATUS_NEW;
	}

	if (!problem_srs->is_mip) {
		switch (problem_srs->problem_simplexe->ExistenceDUneSolution) {
		case OUI_SPX :
			return SRS_STATUS_OPTIMAL;
		default:
			return SRS_STATUS_UNFEASIBLE;
		}
	}
	else {
		switch (problem_srs->problem_mip->ExistenceDUneSolution) {
		case SOLUTION_OPTIMALE_TROUVEE:
			return SRS_STATUS_OPTIMAL;
		case PAS_DE_SOLUTION_TROUVEE:
			return SRS_STATUS_NO_SOL_FOUND;
		case PROBLEME_INFAISABLE:
			return SRS_STATUS_UNFEASIBLE;
		case PROBLEME_NON_BORNE:
			return SRS_STATUS_UNBOUNDED;
		case ARRET_PAR_LIMITE_DE_TEMPS_AVEC_SOLUTION_ADMISSIBLE_DISPONIBLE:
			return SRS_STATUS_TIMEOUT_WITH_SOL;
		default:
			return SRS_STATUS_UNFEASIBLE;
		}
	}
}

int SRSgetdualvalues(SRS_PROBLEM * problem_srs, double ** dualValues) {
	int nbRow = problem_srs->problem_mps->NbCnt;
	if (dualValues != NULL && (*dualValues != NULL)) {
		for (int idxRow = 0; idxRow < nbRow; ++idxRow) {
			if (problem_srs->is_mip)
				(*dualValues)[idxRow] = problem_srs->problem_mip->VariablesDualesDesContraintes[idxRow];
			else
				(*dualValues)[idxRow] = problem_srs->problem_simplexe->CoutsMarginauxDesContraintes[idxRow];
		}
	}
	return 0;
} //getpi PROBLEME_SIMPLEXE CoutsMarginauxDesContraintes / PROBLEME_MIP VariablesDualesDesContraintes ?

int SRSgetreducedcosts(SRS_PROBLEM * problem_srs, double ** reducedCostValues) {
	int nbCol = problem_srs->problem_mps->NbVar;
	if (reducedCostValues != NULL && (*reducedCostValues) != NULL) {
		for (int idxCol = 0; idxCol < nbCol; ++idxCol) {
			(*reducedCostValues)[idxCol] = problem_srs->problem_simplexe->CoutsReduits[idxCol];
		}
	}
	return 0;
}

int SRSgetmipnodecount(SRS_PROBLEM * problem_srs, int * nodeCount) {
	if (problem_srs->problem_mip == NULL) {
		return -1;
	}
	//FIXME (*nodeCount) = problem_srs->problem_mip->NombreDeProblemesResolus;

	return 0;
}

int SRSgetnbcols(SRS_PROBLEM * problem_srs) { return problem_srs->problem_mps->NbVar; }
int SRSgetnbrows(SRS_PROBLEM * problem_srs) { return problem_srs->problem_mps->NbCnt; }

int SRSsetintparams(SRS_PROBLEM * problem_srs, const char * paramId, int paramValue) {
	if (strcmp(SRS_PARAM_VERBOSE_SPX, paramId) == 0) {
		problem_srs->verboseSpx = paramValue;
		return 0;
	}
	else if (strcmp(SRS_PARAM_VERBOSE_PNE, paramId) == 0) {
		problem_srs->verbosePne = paramValue;
		return 0;
	}
	else if (strcmp(SRS_PARAM_MAXIMIZE, paramId) == 0) {
		problem_srs->maximize = (paramValue ? true : false);
		return 0;
	}
	else if (strcmp(SRS_PARAM_PRESOLVE, paramId) == 0) {
		problem_srs->presolve = paramValue;
		return 0;
	}
	else if (strcmp(SRS_PARAM_SCALING, paramId) == 0) {
		problem_srs->scaling = paramValue;
		return 0;
	}
	if (strcmp(SRS_FORCE_PNE, paramId) == 0) {
		problem_srs->is_mip = true;
		return 0;
	}

	fprintf(stderr, "(ERROR) Unknown int parameter : %s\n", paramId);
	return -1;
}

int SRSsetdoubleparams(SRS_PROBLEM * problem_srs, const char * paramId, double paramValue) { 
	if (strcmp(SRS_PARAM_RELATIVE_GAP, paramId) == 0) {
		problem_srs->relativeGap = paramValue;
		return 0;
	}
	else if (strcmp(SRS_PARAM_MAX_TIME, paramId) == 0) {
		problem_srs->maxTime = paramValue;
		return 0;
	}

	fprintf(stderr, "(ERROR) Unknown double parameter : %s\n", paramId);
	return -1;
}
int SRSchgsens(SRS_PROBLEM * problem_srs, size_t nbRowIndexes, const int * rowIndexes, const char * newSense) {
	for (int idx = 0; idx < nbRowIndexes; ++idx) {
		int rowAbsoluteIndex = rowIndexes[idx];
		problem_srs->problem_mps->SensDeLaContrainte[rowAbsoluteIndex] = newSense[idx];
	}

	return 0;
}

int SRSchgrangeval(SRS_PROBLEM * problem_srs, size_t nbRowIndexes, const int * rowIndexes, const double * newRngVal) {
	for (int idx = 0; idx < nbRowIndexes; ++idx) {
		int rowAbsoluteIndex = rowIndexes[idx];
		problem_srs->problem_mps->BRange[rowAbsoluteIndex] = newRngVal[idx];
	}

	return 0;
}

int SRSsetobjoffset(SRS_PROBLEM* problem_srs, double value)
{
    problem_srs->problem_mps->objective_offset = value;
    return 0;
}

double SRSgetobjoffset(const SRS_PROBLEM* problem_srs)
{
    return problem_srs->problem_mps->objective_offset;
}

int SRScopyvarboundstype(SRS_PROBLEM * problem_srs, int * varBoundsTypeValues)
{
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;
	int nbVar = problem_mps->NbVar;
	for (int idxVar = 0; idxVar < nbVar; ++idxVar) {
		problem_mps->TypeDeBorneDeLaVariable[idxVar] = varBoundsTypeValues[idxVar];
	}
}

int SRSsetxvalue(SRS_PROBLEM * problem_srs, int varIndex, double xValue)
{
	PROBLEME_MPS * problem_mps = problem_srs->problem_mps;
	// set x value at the right index
	problem_mps->U[varIndex] = xValue;
}

int SRSgetcolbasisstatus(SRS_PROBLEM * problem_srs, char ** colStatuses) {
	if (problem_srs->problem_simplexe == NULL) {
		return -1;
	}

	int nbVar = problem_srs->problem_mps->NbVar;
	for (int idx = 0; idx < nbVar; ++idx) {
		(*colStatuses)[idx] = problem_srs->problem_simplexe->PositionDeLaVariable[idx];
	}

	return 0;
}

int SRSgetrowbasisstatus(SRS_PROBLEM * problem_srs, char ** rowStatuses) {
	if (problem_srs->problem_simplexe == NULL) {
		return -1;
	}

	int nbRow = problem_srs->problem_mps->NbCnt;
	(*rowStatuses) = malloc(nbRow * sizeof(char));

	for (int idx = 0; idx < nbRow; ++idx) {
		(*rowStatuses)[idx] = EN_BASE_LIBRE;
	}

	for (int idx = 0; idx < problem_srs->problem_simplexe->NbVarDeBaseComplementaires; ++idx)
	{
		(*rowStatuses)[problem_srs->problem_simplexe->ComplementDeLaBase[idx]] = EN_BASE;
	}

	return 0;
}

int SRSgetbestbound(SRS_PROBLEM * problem_srs, double * bestBoundVal) {
	if (problem_srs->is_mip) {
		int status = SRSgetproblemstatus(problem_srs);
		if (status == SRS_STATUS_OPTIMAL || status == SRS_STATUS_TIMEOUT_WITH_SOL)
		{
			//FIXME (*bestBoundVal) = problem_srs->problem_mip->ValeurDuMeilleurMinorant;
			return 0;
		}
		fprintf(stderr, "(ERROR) no solution found, can't get a best bound\n");
	}

	return -1;
}

int SRSsetdefaultcbmessage(SRS_PROBLEM *problem_srs, callback_function the_callback_function, SIRIUS_LOGLEVEL log_level)
{
	problem_srs->callback = SRSdefault_callback;
}
int SRSsetcbmessage(SRS_PROBLEM *problem_srs, callback_function the_callback_function, void *caller, SIRIUS_LOGLEVEL log_level)
{
	problem_srs->callback = the_callback_function;
	problem_srs->caller = caller;
}

# ifdef __cplusplus
}
# endif
