// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Calcul de NBarre pour la variable sortante dans le cas
	           hyper creux.
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

# include "lu_define.h"

# define POSITIF 1
# define NEGATIF 2
# define POSITIF_ET_NEGATIF 3

/*----------------------------------------------------------------------------*/

void SPX_DualCalculerNBarreRHyperCreux( PROBLEME_SPX * Spx )
{
int Var; int il; int ilMax; double X; int i; double * ErBMoinsUn; int Cnt; int * Indcol;
double * A; int j; int * T; char * PositionDeLaVariable; int * IndexTermesNonNulsDeErBMoinsUn;
int NbTermesNonNulsDeErBMoinsUn; int * NumVarNBarreRNonNul; int NombreDeValeursNonNullesDeNBarreR;
double * ACol; int * Cdeb; int * CNbTerm;	int * NumeroDeContrainte; int * NumerosDesVariablesHorsBase;
double * NBarreR; double * Erb; int * Mdeb; int * NbTerm; int Seuil; int * NombreDeVariablesHorsBaseDeLaContrainte;
double * Xmin; double * Xmax; int jFin; int * IndicesDeLigne; int * LigneDeLaBaseFactorisee;

ErBMoinsUn = Spx->ErBMoinsUn;
IndexTermesNonNulsDeErBMoinsUn = Spx->IndexTermesNonNulsDeErBMoinsUn;
NBarreR = Spx->NBarreR;									
NbTermesNonNulsDeErBMoinsUn = Spx->NbTermesNonNulsDeErBMoinsUn;

if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) Seuil = Spx->RangDeLaMatriceFactorisee;
else Seuil = Spx->NombreDeContraintes;

Seuil = Spx->NombreDeContraintes; /* On reste comme ca pour l'instant */

if ( NbTermesNonNulsDeErBMoinsUn < 0.3 * Seuil || Spx->UtiliserLaBaseReduite == OUI_SPX ) { /* Il semble qu'il vaut mieux toujours faire ca si base reduite */
  /* Cette methode pose des problemes de precision car il y a plus d'allers
	   retour en memoire or a chaque aller retour il y a un ecretage */
  Spx->TypeDeStockageDeNBarreR = ADRESSAGE_INDIRECT_SPX;
	
  Mdeb   = Spx->Mdeb;
  NbTerm = Spx->NbTerm;
  Indcol = Spx->Indcol; 
  A      = Spx->A;

	PositionDeLaVariable = Spx->PositionDeLaVariable;

	NombreDeVariablesHorsBaseDeLaContrainte = Spx->NombreDeVariablesHorsBaseDeLaContrainte;

  T = Spx->T;
  NumVarNBarreRNonNul = Spx->NumVarNBarreRNonNul;
  NombreDeValeursNonNullesDeNBarreR = 0;
	
	/* Il faut supprimer les variables pour lesquelles Xmin = Xmax */		
	Xmin = Spx->Xmin;
	Xmax = Spx->Xmax;
	
	for ( j = 0 ; j < NbTermesNonNulsDeErBMoinsUn ; j++ ) {
    Cnt = IndexTermesNonNulsDeErBMoinsUn[j];
		if ( NombreDeVariablesHorsBaseDeLaContrainte[Cnt] == 0 ) continue; 
    X = ErBMoinsUn[j];		
    il = Mdeb[Cnt];
    ilMax = il + NbTerm[Cnt];
    while ( il < ilMax ) {
	    Var = Indcol[il];			
			if ( PositionDeLaVariable[Var] != EN_BASE_LIBRE ) {
        if ( T[Var] == 1 ) {
			    NBarreR[Var] += X * A[il];
        }
			  else {									
			    T[Var] = 1;
		 	    NBarreR[Var] = X * A[il];
			 	  NumVarNBarreRNonNul[NombreDeValeursNonNullesDeNBarreR] = Var;
          NombreDeValeursNonNullesDeNBarreR++;					
			  }
			}
      il++;
		}
	}
	
  for ( i = 0 ; i < NombreDeValeursNonNullesDeNBarreR ; i++ ) T[NumVarNBarreRNonNul[i]] = -1;	

	/* Il faut supprimer les variables pour lesquelles Xmin = Xmax */
	if ( Spx->PresenceDeVariablesDeBornesIdentiques == OUI_SPX ) {		
    j = 0;
	  jFin = NombreDeValeursNonNullesDeNBarreR - 1;		
	  while ( j <= jFin ) {
	    Var = NumVarNBarreRNonNul[j];						
	    if ( Xmin[Var] == Xmax[Var] ) {				
	      NumVarNBarreRNonNul[j] = NumVarNBarreRNonNul[jFin];
		    jFin--;
		 	  continue;
	    }
		  j++;
	  }
	  NombreDeValeursNonNullesDeNBarreR = jFin + 1;
	}

  Spx->NombreDeValeursNonNullesDeNBarreR = NombreDeValeursNonNullesDeNBarreR;
			
}
else {	
  Spx->TypeDeStockageDeNBarreR = ADRESSAGE_INDIRECT_SPX /*VECTEUR_SPX*/;
  NumVarNBarreRNonNul = Spx->NumVarNBarreRNonNul;
  NombreDeValeursNonNullesDeNBarreR = 0;
	
	/* Expand de ErBMoinsUn */
  Erb = Spx->Bs;  /* On peut aussi utiliser Spx->AReduit */
	
	for ( j = 0 ; j < NbTermesNonNulsDeErBMoinsUn ; j++ ) Erb[IndexTermesNonNulsDeErBMoinsUn[j]] = ErBMoinsUn[j];
 	ErBMoinsUn = Erb;

	NumerosDesVariablesHorsBase = Spx->NumerosDesVariablesHorsBase;

  if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
    Cdeb = Spx->CdebProblemeReduit;
    CNbTerm = Spx->CNbTermProblemeReduit;
    IndicesDeLigne = Spx->IndicesDeLigneDesTermesDuProblemeReduit;
    ACol = Spx->ValeurDesTermesDesColonnesDuProblemeReduit;			
	  LigneDeLaBaseFactorisee = Spx->LigneDeLaBaseFactorisee;
    for ( i = 0 ; i < Spx->NombreDeVariablesHorsBase ; i++ ) {
	    Var = NumerosDesVariablesHorsBase[i];					
      il    = Cdeb[Var];
      ilMax = il + CNbTerm[Var];
      X = 0.0;
      while ( il < ilMax ) {
        X += ErBMoinsUn[LigneDeLaBaseFactorisee[IndicesDeLigne[il]]] * ACol[il];
        il++;
      }			 		
		  if ( X != 0.0 ) {
        NBarreR[Var] = X;
			  NumVarNBarreRNonNul[NombreDeValeursNonNullesDeNBarreR] = Var;
        NombreDeValeursNonNullesDeNBarreR++;			
		  }
    }
	}
	else {		
    Cdeb = Spx->Cdeb;
    CNbTerm = Spx->CNbTerm;
    NumeroDeContrainte = Spx->NumeroDeContrainte;
    ACol = Spx->ACol;

    for ( i = 0 ; i < Spx->NombreDeVariablesHorsBase ; i++ ) {
	    Var = NumerosDesVariablesHorsBase[i];					
      il    = Cdeb[Var];
      ilMax = il + CNbTerm[Var];
      X = 0.0;
      while ( il < ilMax ) {
        X += ErBMoinsUn[NumeroDeContrainte[il]] * ACol[il];
        il++;
      }			 		
		  if ( X != 0.0 ) {
        NBarreR[Var] = X;
			  NumVarNBarreRNonNul[NombreDeValeursNonNullesDeNBarreR] = Var;
        NombreDeValeursNonNullesDeNBarreR++;			
		  }
    }
	}
			
  Spx->NombreDeValeursNonNullesDeNBarreR = NombreDeValeursNonNullesDeNBarreR;	
	for ( j = 0 ; j < NbTermesNonNulsDeErBMoinsUn ; j++ ) Erb[IndexTermesNonNulsDeErBMoinsUn[j]] = 0.0;
	
}
			
return;
}





