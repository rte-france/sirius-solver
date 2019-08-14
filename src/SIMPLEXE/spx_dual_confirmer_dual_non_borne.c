// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Confirmation dual non borne (i.e. pas de solution) 
            
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

# include "lu_define.h"
  
//# define Spx->spx_params->VERBOSE_SPX 0
  
/*----------------------------------------------------------------------------*/

void SPX_DualConfirmerDualNonBorne( PROBLEME_SPX * Spx )

{
double NBarreR; double X; /* char PositionDeLaVariable; */ double MinRapport; double CBarreTPlus;
double CBarreTMoins; double NBarreRTPlus; double NBarreRTMoins; double S; double SeuilDePivot;
double Violation; int CntChoix; int Cnt; int j; double * ErBMoinsUn; double * Erb; /* int   Var; int il ; int ilMax; */
char NBarreRInitialise; double Seuil ; int Index;

//if (Spx->spx_params->PRICING_AVEC_VIOLATIONS_STRICTES == OUI_SPX){
	int IndexMax; int Var; int YaDesBornesViolees; double ValBBarre; double * Xmax; double * SeuilDeViolationDeBorne;
	int * VariableEnBaseDeLaContrainte; int * ColonneDeLaBaseFactorisee; char * TypeDeVariable; double * BBarre;
//}

Spx->AdmissibilitePossible = NON_SPX;

if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
  Index = Spx->OrdreColonneDeLaBaseFactorisee[Spx->ContrainteDeLaVariableEnBase[Spx->VariableSortante]];
}
else {
  Index = Spx->ContrainteDeLaVariableEnBase[Spx->VariableSortante];
}

/* Si la variable sortante viole trop ses bornes, on ne cherche pas plus loin */
if ( Spx->BBarre[Index] < Spx->Xmin[Spx->VariableSortante] ) {
  Violation = Spx->Xmin[Spx->VariableSortante] - Spx->BBarre[Index];
}
else Violation = Spx->BBarre[Index] - Spx->Xmax[Spx->VariableSortante];

if ( fabs( Violation ) > Spx->spx_params->SEUIL_MAX_DE_VIOLATION_DE_BORNE ) {
	if (Spx->spx_params->VERBOSE_SPX){
		printf("Violation trop importante %e on confirme le dual non borne, poids %e seuil de violation %e  SeuilDePivotDual %e\n", Violation,
			Spx->DualPoids[Index], Spx->SeuilDeViolationDeBorne[Spx->VariableSortante], Spx->SeuilDePivotDual);
	}
  return;
}


if (Spx->spx_params->PRICING_AVEC_VIOLATIONS_STRICTES == OUI_SPX) {
	Xmax = Spx->Xmax;
	SeuilDeViolationDeBorne = Spx->SeuilDeViolationDeBorne;
	VariableEnBaseDeLaContrainte = Spx->VariableEnBaseDeLaContrainte;
	ColonneDeLaBaseFactorisee = Spx->ColonneDeLaBaseFactorisee;
	TypeDeVariable = Spx->TypeDeVariable;
	BBarre = Spx->BBarre;

	/* Si on travaille avec une gestion des tolerance nulles pour la liste des variables en base a controler, on verifie les valeurs
	   par rapport aux tolerance. Si elles sont toutes respectees alors on a une solution admissible optimale */
	if (Spx->UtiliserLaBaseReduite == OUI_SPX) IndexMax = Spx->RangDeLaMatriceFactorisee;
	else IndexMax = Spx->NombreDeContraintes;
	YaDesBornesViolees = NON_SPX;
	for (Index = 0; Index < IndexMax; Index++) {
		if (Spx->UtiliserLaBaseReduite == OUI_SPX) {
			Var = VariableEnBaseDeLaContrainte[ColonneDeLaBaseFactorisee[Index]];
		}
		else {
			Var = VariableEnBaseDeLaContrainte[Index];
		}
		if (TypeDeVariable[Var] == NON_BORNEE) continue;

		ValBBarre = BBarre[Index];
		if (ValBBarre < -SeuilDeViolationDeBorne[Var]) {
			YaDesBornesViolees = OUI_SPX;
			break;
		}
		if (TypeDeVariable[Var] == VARIABLE_BORNEE_DES_DEUX_COTES) {
			if (ValBBarre > Xmax[Var] + SeuilDeViolationDeBorne[Var]) {
				YaDesBornesViolees = OUI_SPX;
				break;
			}
		}
	}

	if (YaDesBornesViolees == NON_SPX) {
		Spx->AdmissibilitePossible = OUI_SPX;
		return;
	}
}

SeuilDePivot = Spx->spx_params->VALEUR_DE_PIVOT_ACCEPTABLE;
NBarreRInitialise = NON_SPX;
NBarreR = 1; /* Pour ne pas avoir de warning a la compilation */

if (Spx->spx_params->VERBOSE_SPX) {
	printf("Recherche des variables entrantes TPlus et TMoins pour confirmer le dual non borne\n"); fflush(stdout);
}

Erb = NULL;
if ( Spx->TypeDeStockageDeErBMoinsUn != COMPACT_SPX ) {
  ErBMoinsUn = Spx->ErBMoinsUn;
}
else {
	Erb = Spx->Bs; /* On peut aussi utiliser Spx->AReduit */	
	for ( j = 0 ; j < Spx->NbTermesNonNulsDeErBMoinsUn ; j++ ) {
	  Erb[Spx->IndexTermesNonNulsDeErBMoinsUn[j]] = Spx->ErBMoinsUn[j];
	}
 	ErBMoinsUn = Erb;
}
	
/* Norme infinie */
for ( S = -1., Cnt = 0 ; Cnt < Spx->NombreDeContraintes ; Cnt++ ) {
    if ( fabs( ErBMoinsUn[Cnt] ) > S ) S = fabs( ErBMoinsUn[Cnt] );
}
S = 1. / S;

/* Le vecteur dual */
for ( Cnt = 0 ; Cnt < Spx->NombreDeContraintes ; Cnt++ ) Spx->Pi[Cnt] = -ErBMoinsUn[Cnt] * S;

/* Si la seule variable qui peut sortir est une variable t alors on est bien non admissible */
MinRapport = LINFINI_SPX;
CntChoix = -1;
for ( Cnt = 0 ; Cnt < Spx->NombreDeContraintes ; Cnt++ ) {
  CBarreTPlus  = 1. - Spx->Pi[Cnt];
  CBarreTMoins = 1. + Spx->Pi[Cnt];
  if ( CBarreTPlus < 0. || CBarreTMoins < 0. ) {
    printf("Iteration %d CBarreTPlus %lf CBarreTMoins %lf Cnt %d\n",Spx->Iteration,CBarreTPlus,CBarreTMoins,Cnt);
  }
  NBarreRTPlus  = ErBMoinsUn[Cnt];
  NBarreRTMoins = -ErBMoinsUn[Cnt];		
  if ( Spx->SortSurXmaxOuSurXmin == SORT_SUR_XMIN ) {
    /* Si le produit CBarre * NBarreR est négatif la variable peut rentre en base */		
    if ( fabs( NBarreRTPlus ) >= SeuilDePivot ) { 
      X = CBarreTPlus / NBarreRTPlus;
      if ( X < 0. && fabs( X ) < MinRapport ) { 
        /*printf("Contrainte %d la variable TPlus peut entrer en base, CBarreTPlus %lf NBarreRTPlus %lf\n",Cnt,CBarreTPlus,NBarreRTPlus);*/
        MinRapport = fabs( X ); 
        NBarreR    = NBarreRTPlus;
	      NBarreRInitialise = OUI_SPX;
				CntChoix = Cnt;
      }
    }
    if ( fabs( NBarreRTMoins ) >= SeuilDePivot ) {
      X = CBarreTMoins / NBarreRTMoins;
      if ( X < 0. && fabs( X ) < MinRapport ) {
        /*printf("Contrainte %d la variable TMoins peut entrer en base, CBarreTMoins %lf NBarreRTMoins %lf\n",Cnt,CBarreTMoins,NBarreRTMoins);*/
        MinRapport = fabs( X ); 
        NBarreR    = NBarreRTMoins;
	      NBarreRInitialise = OUI_SPX;
				CntChoix = Cnt;
      }
    }
  }
  else if ( Spx->SortSurXmaxOuSurXmin == SORT_SUR_XMAX ) {
    /* Si le produit CBarre * NBarreR est positif la variable peut rentre en base */				
    if ( fabs( NBarreRTPlus ) >= SeuilDePivot ) { 
      X = CBarreTPlus / NBarreRTPlus;
      if ( X > 0. && X < MinRapport ) { 
        /*printf("Contrainte %d la variable TPlus peut entrer en base, CBarreTPlus %lf NBarreRTPlus %lf\n",Cnt,CBarreTPlus,NBarreRTPlus);*/
        MinRapport = X; 
        NBarreR    = NBarreRTPlus;
	      NBarreRInitialise = OUI_SPX;
				CntChoix = Cnt;
      }
    }		
    if ( fabs( NBarreRTMoins ) >= SeuilDePivot ) {
      X = CBarreTMoins / NBarreRTMoins;
      if ( X > 0. && X < MinRapport ) {
        /*printf("Contrainte %d la variable TMoins peut entrer en base, CBarreTMoins %lf NBarreRTMoins %lf\n",Cnt,CBarreTMoins,NBarreRTMoins);*/
        MinRapport = X;  
        NBarreR    = NBarreRTMoins;
	      NBarreRInitialise = OUI_SPX;
				CntChoix = Cnt;
      }
    }
  }
}

if ( NBarreRInitialise == OUI_SPX && CntChoix >= 0 ) {
  if ( Spx->SortSurXmaxOuSurXmin == SORT_SUR_XMIN ) {
    Spx->DeltaXSurLaVariableHorsBase = Spx->BBarre[Index] / NBarreR;
  }
  else { /* SORT_SUR_XMAX */
    Spx->DeltaXSurLaVariableHorsBase = ( Spx->BBarre[Index] - Spx->Xmax[Spx->VariableSortante] ) / NBarreR;
  }

	/* Les variables t representent des variables d'ecart i.e. des possibilites de violation des contraintes */
	/*
  Seuil = Spx->spx_params->SEUIL_DE_VIOLATION_DE_BORNE_NON_NATIVE * Spx->ScaleB[CntChoix];
  if ( Seuil < Spx->spx_params->SEUIL_MIN_DE_VIOLATION_DE_BORNE_NON_NATIVE ) Seuil = Spx->spx_params->SEUIL_MIN_DE_VIOLATION_DE_BORNE_NON_NATIVE;
  else if ( Seuil > Spx->spx_params->SEUIL_MAX_DE_VIOLATION_DE_BORNE_NON_NATIVE ) Seuil = Spx->spx_params->SEUIL_MAX_DE_VIOLATION_DE_BORNE_NON_NATIVE;
	*/
	
  Seuil = Spx->spx_params->SEUIL_DADMISSIBILITE * Spx->ScaleB[CntChoix];	 
	if ( Seuil < 0.01 * Spx->spx_params->SEUIL_DADMISSIBILITE ) Seuil = 0.01 * Spx->spx_params->SEUIL_DADMISSIBILITE;  
  else if ( Seuil > 10 * Spx->spx_params->SEUIL_DADMISSIBILITE ) Seuil = 10 * Spx->spx_params->SEUIL_DADMISSIBILITE;	  
	
  if (Spx->spx_params->VERBOSE_SPX) {
	  printf("Type de sortie %d Delta X a faire sur T %15.10lf Seuil %e\n", Spx->SortSurXmaxOuSurXmin, Spx->DeltaXSurLaVariableHorsBase,
		  Seuil);
  }
		
  if ( fabs( Spx->DeltaXSurLaVariableHorsBase ) < Seuil ) {
    Spx->AdmissibilitePossible = OUI_SPX;		
	if (Spx->spx_params->VERBOSE_SPX) {
		printf("Rectification du diagnostique: admissibilite possible\n");
		printf("Iteration %d DeltaXSurLaVariableHorsBase %e SeuilDeViolationDeBorne %e Spx->spx_params->SEUIL_DE_VIOLATION_DE_BORNE %e\n",
			Spx->Iteration,
			Spx->DeltaXSurLaVariableHorsBase,
			Spx->SeuilDeViolationDeBorne[Spx->VariableSortante],
			Spx->spx_params->SEUIL_DE_VIOLATION_DE_BORNE);
		printf("             Variable sortante:  Xmin %e X %e Xmax %e\n",
			Spx->Xmin[Spx->VariableSortante],
			Spx->BBarre[Index],
			Spx->Xmax[Spx->VariableSortante]);
	}
  }
	else {
		if (Spx->spx_params->VERBOSE_SPX) {
			printf("On confirme le dual non borne\n");
			printf("Iteration %d DeltaXSurLaVariableHorsBase %e SeuilDeViolationDeBorne %e Spx->spx_params->SEUIL_DE_VIOLATION_DE_BORNE %e\n",
				Spx->Iteration,
				Spx->DeltaXSurLaVariableHorsBase,
				Spx->SeuilDeViolationDeBorne[Spx->VariableSortante],
				Spx->spx_params->SEUIL_DE_VIOLATION_DE_BORNE);
			printf("             Variable sortante:  Xmin %e X %e Xmax %e\n",
				Spx->Xmin[Spx->VariableSortante],
				Spx->BBarre[Index],
				Spx->Xmax[Spx->VariableSortante]);
		}
	}
}
else {
  if ( fabs( Violation ) > Spx->spx_params->SEUIL_DE_VIOLATION_DE_BORNE ) {
	  if (Spx->spx_params->VERBOSE_SPX) {
		  printf("On confirme le dual non borne NBarreRInitialise %d CntChoix %d\n", NBarreRInitialise, CntChoix);
	  }
	}
	else {
		if (Spx->spx_params->VERBOSE_SPX) {
			printf("Rectification du diagnostique: admissibilite possible\n");
		}
    Spx->AdmissibilitePossible = OUI_SPX;
	}
}

/* Raz eventuelle du vecteur ayant servi pour Erb */
if ( Spx->TypeDeStockageDeErBMoinsUn == COMPACT_SPX && Erb != NULL ) {
	for ( j = 0 ; j < Spx->NbTermesNonNulsDeErBMoinsUn ; j++ ) Erb[Spx->IndexTermesNonNulsDeErBMoinsUn[j]] = 0.0;
}

return;
}




