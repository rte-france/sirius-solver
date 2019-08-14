// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Main de la SPX pour le standalone
                
   AUTEUR: R. GONZALEZ, N. LHUILLIER

************************************************************************/

# define CPLUSPLUS
  # undef CPLUSPLUS

# include "spx_sys.h"
# include "pne_fonctions.h"

# ifdef __cplusplus
  # include "spx_definition_arguments.h"
  # include "spx_constantes_externes.h"	       						 
  # include "spx_define.h"
  extern "C"
  {
  PROBLEME_SPX * SPX_Simplexe( PROBLEME_SIMPLEXE * Probleme , PROBLEME_SPX * Spx );
  void PNE_LireJeuDeDonneesMPS(void);
  }
# else
  # include "spx_fonctions.h"
  # include "spx_constantes_externes.h"	       						 
  # include "spx_define.h"
# endif

# include "mps_define.h"
# include "mps_extern_global.h"
#include <time.h>
#define difftimens(ts2, ts1) (double)ts2.tv_sec - (double)ts1.tv_sec + ((double)ts2.tv_nsec - (double)ts1.tv_nsec)/1.e9


PROBLEME_SIMPLEXE probleme;
PROBLEME_SPX *resultat;

PROBLEME_MPS * readAndOpt(char * mpsName, PROBLEME_MPS * Mps, FILE * FlotDeSortie, PROBLEME_SPX ** Spx, int warmstart)
{
	static char * previousMpsName = NULL;
	int j = 0, i = 0;
	/* Mip: indicateur positionne a OUI_MPS si on veut optimiser en variables entieres
			et positionne a NON_MPS si on veut juste faire un simplexe */

	/* Resolution */
	PNE_LireJeuDeDonneesMPS_AvecNom(Mps, mpsName);
	for (j = 0; j < Mps->NbVar; j++) Mps->TypeDeVariable[j] = REEL;
	probleme.TypeDePricing = PRICING_STEEPEST_EDGE;//PRICING_STEEPEST_EDGE PRICING_DANTZIG()
	probleme.FaireDuScaling = OUI_SPX; // Vaut OUI_SPX ou NON_SPX
	probleme.StrategieAntiDegenerescence = AGRESSIF; // Vaut AGRESSIF ou PEU_AGRESSIF
	probleme.NombreMaxDIterations = -1; // si i < 0 , alors le simplexe prendre sa valeur par defaut
	probleme.DureeMaxDuCalcul = -1; // si i < 0 , alors le simplexe prendre sa valeur par defaut
	probleme.CoutLineaire = Mps->CoefsObjectif;
	probleme.X = Mps->U;
	probleme.Xmin = Mps->Umin;
	probleme.Xmax = Mps->Umax;
	probleme.NombreDeVariables = Mps->NbVar;
	probleme.TypeDeVariable = Mps->TypeDeBorneDeLaVariable;
	probleme.NombreDeContraintes = Mps->NbCnt;
	probleme.IndicesDebutDeLigne = Mps->Mdeb;
	probleme.NombreDeTermesDesLignes = Mps->NbTerm;
	probleme.IndicesColonnes = Mps->Nuvar;
	probleme.CoefficientsDeLaMatriceDesContraintes = Mps->A;
	probleme.Sens = Mps->SensDeLaContrainte;
	probleme.SecondMembre = Mps->Rhs;
	probleme.CoutsMarginauxDesContraintes = Mps->VariablesDualesDesContraintes;
	probleme.ChoixDeLAlgorithme = SPX_DUAL;

	probleme.LibererMemoireALaFin = NON_SPX;
	probleme.AffichageDesTraces = OUI_SPX;
	probleme.CoutMax = -1;
	probleme.UtiliserCoutMax = NON_SPX;

	probleme.Contexte = SIMPLEXE_SEUL;
	probleme.BaseDeDepartFournie = NON_SPX;

	//Instantiation du resultat
	probleme.ComplementDeLaBase = (int*)malloc(Mps->NbCnt * sizeof(int));
	probleme.PositionDeLaVariable = (int*)malloc(Mps->NbVar * sizeof(int));
	probleme.CoutsReduits = (double*)malloc(Mps->NbVar * sizeof(double));
	if ((*Spx) != NULL)
	{
		SPX_ModifierLeVecteurCouts(*Spx, Mps->CoefsObjectif, probleme.NombreDeVariables);
		SPX_ModifierLeVecteurSecondMembre(*Spx, Mps->Rhs, Mps->SensDeLaContrainte, probleme.NombreDeContraintes);

		probleme.Contexte = BRANCH_AND_BOUND_OU_CUT_NOEUD;
		probleme.BaseDeDepartFournie = UTILISER_LA_BASE_DU_PROBLEME_SPX;
	}

	// Appel du simplexe
	struct timespec debut;
	timespec_get(&debut, TIME_UTC);
	(*Spx) = SPX_Simplexe(&probleme, (*Spx), NULL);
	struct timespec fin;
	timespec_get(&fin, TIME_UTC);
	double TempsEcoule = difftimens(fin, debut);

	int YaUneSolution = probleme.ExistenceDUneSolution;

	int Nbn = 0, Cnt = 0;
	printf("Dual variables count %d\n", Mps->NbCnt);
	for (Cnt = 0; Cnt < Mps->NbCnt; Cnt++) {
		if (fabs(Mps->VariablesDualesDesContraintes[Cnt]) < 1.e-8) Nbn++;
	}
	printf("Zero dual variables count %d over %d\n", Nbn, Mps->NbCnt);

	/* Resultats */
	if (YaUneSolution == NON_SPX) printf("No solution found\n");
	if (YaUneSolution == SPX_ERREUR_INTERNE) printf("Internal error\n");
	if (YaUneSolution == SPX_MATRICE_DE_BASE_SINGULIERE) printf("Problem is infeasible\n");
	if (YaUneSolution == OUI_SPX) {
		double Critere = 0.;
		for (i = 0; i < Mps->NbVar; i++) {
			Critere += Mps->CoefsObjectif[i] * Mps->U[i];
			fprintf(FlotDeSortie, "%s;%e\n", Mps->LabelDeLaVariable[i], Mps->U[i]);
		}
		//int iter = probleme
		//printf("\n********** Optimal solution found, objective %e **********\n\n", Critere);
		printf("mps:%s,status:1,obj:%.10e,time:%e,iter:%d,hot:%s\n",
			mpsName,
			Critere,
			TempsEcoule,
			(*Spx)->Iteration,
			((warmstart == 0) ? "cold" : previousMpsName)
		);
	}
	else {
		printf("mps:%s,status:-9,obj:%.10e,time:%e,iter:%d,hot:%s\n",
			mpsName,
			-9.0,
			TempsEcoule,
			(*Spx)->Iteration,
			((warmstart == 0) ? "cold" : previousMpsName)
		);
	}

	if (warmstart == 0) {
		if (Spx != NULL) {
			SPX_LibererProbleme(*Spx);
			(*Spx) = NULL;
		}
	}
	
	previousMpsName = mpsName;

	return Mps;
}


int main( int argc , char ** argv ) {
FILE * FlotDeSortie; 

int firstMpsIndex = 3;
int firstHotstartIndex = firstMpsIndex + 1;
if (argc < firstHotstartIndex)
{
	printf("usage ANTARES.exe {presolve: 1 ou 0} {algo: primal, dual ou barrier} {warmstart: 1 ou 0} fichier.mps [fichierHotstart.mps] [fichierHotstart.mps] [fichierHotstart.mps] ...\n");
	return -1;
}

int warmstart = (strcmp(argv[3], "1") == 0 ? 1 : 0);

char ** hotstartFiles = (char**)malloc(argc*sizeof(char*));
int idxHotFile = 0;
for (int idxArg = firstHotstartIndex; idxArg < argc; ++idxArg)
	hotstartFiles[idxHotFile++]=argv[idxArg];
int nbHotstartFiles = idxHotFile;

 fopen_s(&FlotDeSortie, "RESULTAT.csv", "w" );
if( FlotDeSortie == NULL ) {
  printf("Erreur ouverture du fichier de resultats \n");
  exit(0);
}

/* Lecture du jeu de donnees */
/*
printf("Debut de la lecture des donnees \n"); fflush(stdout);
*/
//PROBLEME_MPS currentMps;
PROBLEME_MPS Mps;
PROBLEME_SPX * Spx = NULL;
//PNE_LireJeuDeDonneesMPS(&Mps);
printf("warmstart = %d\n", warmstart);
for (idxHotFile = 0; idxHotFile < nbHotstartFiles; ++idxHotFile)
{
	if (warmstart == 0)
	{
		if (Spx != NULL) {
			free(Spx);
		}
		Spx = NULL;
	}
	readAndOpt(hotstartFiles[idxHotFile], &Mps, FlotDeSortie, &Spx, warmstart);
}
printf("warmstart = %d\n", warmstart);

return 0;
}
