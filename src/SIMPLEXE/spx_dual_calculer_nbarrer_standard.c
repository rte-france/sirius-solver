// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Calcul de NBarre pour la variable sortante
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

# define COEFF_VARIABLES   0.75

/*----------------------------------------------------------------------------*/

void SPX_DualCalculerNBarreRStandard( PROBLEME_SPX * Spx )

{
int il; int ilMax; double X; int i; int NombreDeContraintes;  int * NumerosDesVariablesHorsBase;
double * ErBMoinsUn; double * NBarreR; int * Cdeb; int * CNbTerm; double * ACol;
int * NumeroDeContrainte; int Cnt; int * Mdeb; int * NbTerm; int * Indcol; double * A;
int NombreDeVariablesHorsBase; int NombreDeVariables; char Methode; int Var;
char * PositionDeLaVariable; int * NombreDeVariablesHorsBaseDeLaContrainte;
int * IndicesDeLigne; int * LigneDeLaBaseFactorisee;

NombreDeContraintes         = Spx->NombreDeContraintes;   
ErBMoinsUn                  = Spx->ErBMoinsUn; 
NombreDeVariables           = Spx->NombreDeVariables;
NombreDeVariablesHorsBase   = Spx->NombreDeVariablesHorsBase;
NumerosDesVariablesHorsBase = Spx->NumerosDesVariablesHorsBase;
NBarreR                     = Spx->NBarreR;   

Spx->TypeDeStockageDeNBarreR = VECTEUR_SPX;

/* Si ErBMoinsUn est tres creux il vaut mieux la methode 1, sinon la methode 2 permet de faire
   un calcul plus precis */

if ( Spx->InverseProbablementDense == OUI_SPX ) {
  Methode = 2;
}
else {
  if ( NombreDeContraintes < (int) ceil ( COEFF_VARIABLES * NombreDeVariables ) ) {
    Methode = 1;
  }
  else { 
    Methode = 2;
  }
}

/* 2 methodes de calcul des produits scalaires */

if ( Methode == 1 ) {
  /*memset( (char *) NBarreR , 0 , NombreDeVariables * sizeof( double ) );*/
	for ( i = 0 ; i < NombreDeVariables ; i++ ) NBarreR[i] = 0;
	
  Mdeb   = Spx->Mdeb;
  NbTerm = Spx->NbTerm;

	PositionDeLaVariable = Spx->PositionDeLaVariable;
	
	NombreDeVariablesHorsBaseDeLaContrainte = Spx->NombreDeVariablesHorsBaseDeLaContrainte;
	
  Indcol = Spx->Indcol;
  A      = Spx->A;
  for ( Cnt = 0 ; Cnt < NombreDeContraintes ; Cnt++ ) {
    if ( ErBMoinsUn[Cnt] == 0.0 ) continue;
		if ( NombreDeVariablesHorsBaseDeLaContrainte[Cnt] == 0 ) continue;		
    X  = ErBMoinsUn[Cnt];		
    il = Mdeb[Cnt];
    ilMax = il + NbTerm[Cnt];
    while ( il < ilMax ) {
			if ( PositionDeLaVariable[Indcol[il]] != EN_BASE_LIBRE ) {
			  NBarreR[Indcol[il]] += X * A[il];
			}
      il++;
		}		    		
  }	
}
else {
  if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
    Cdeb = Spx->CdebProblemeReduit;
    CNbTerm = Spx->CNbTermProblemeReduit;
    IndicesDeLigne = Spx->IndicesDeLigneDesTermesDuProblemeReduit;
    ACol = Spx->ValeurDesTermesDesColonnesDuProblemeReduit;			
	  LigneDeLaBaseFactorisee = Spx->LigneDeLaBaseFactorisee;
    for ( i = 0 ; i < NombreDeVariablesHorsBase ; i++ ) {
	    Var = NumerosDesVariablesHorsBase[i];
      il    = Cdeb[Var];				
      ilMax = il + CNbTerm[Var];
      X = 0;
      while ( il < ilMax ) {
        X += ErBMoinsUn[LigneDeLaBaseFactorisee[IndicesDeLigne[il]]] * ACol[il];
        il++;
      }			 
      NBarreR[Var] = X;		
    }			
  }
  else {
    Cdeb = Spx->Cdeb;
    CNbTerm = Spx->CNbTerm;
    NumeroDeContrainte = Spx->NumeroDeContrainte;
    ACol = Spx->ACol;

    for ( i = 0 ; i < NombreDeVariablesHorsBase ; i++ ) {
	    Var = NumerosDesVariablesHorsBase[i];
      il    = Cdeb[Var];				
      ilMax = il + CNbTerm[Var];
      X = 0;
      while ( il < ilMax ) {
        X += ErBMoinsUn[NumeroDeContrainte[il]] * ACol[il];
        il++;
      }			 
      NBarreR[Var] = X;		
    }				
  }
}

return;
}



