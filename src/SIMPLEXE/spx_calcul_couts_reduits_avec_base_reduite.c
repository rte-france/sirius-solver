// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Calcul des couts reduits des variables hors base 
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_define.h"
# include "spx_fonctions.h"

/*----------------------------------------------------------------------------*/

void SPX_CalculerLesCoutsReduitsAvecBaseReduite( PROBLEME_SPX * Spx )
{
int Var; int il; int ilMax; double S; int i; int * NumerosDesVariablesHorsBase;
int * Cdeb; int * CNbTerm; int * NumeroDeContrainte; double * Pi; double * ACol;
double * C; double * CBarre; int * LigneDeLaBaseFactorisee; int * IndicesDeLigneDesTermesDuProblemeReduit;

NumerosDesVariablesHorsBase = Spx->NumerosDesVariablesHorsBase;
Cdeb    = Spx->CdebProblemeReduit;
CNbTerm = Spx->CNbTermProblemeReduit;
IndicesDeLigneDesTermesDuProblemeReduit = Spx->IndicesDeLigneDesTermesDuProblemeReduit;
ACol    = Spx->ValeurDesTermesDesColonnesDuProblemeReduit;
LigneDeLaBaseFactorisee = Spx->LigneDeLaBaseFactorisee;

Pi      = Spx->Pi;
CBarre  = Spx->CBarre;
C       = Spx->C;
NumeroDeContrainte = Spx->NumeroDeContrainte;

/* Boucle sur les variables hors base */

for ( i = 0 ; i < Spx->NombreDeVariablesHorsBase ; i++ ) {
  Var = NumerosDesVariablesHorsBase[i];
  /* Utilisation du chainage de la transposee de la matrice 
     des contraintes */
  il    = Cdeb[Var];
  ilMax = il + CNbTerm[Var];
  S     = 0.;
  while ( il < ilMax ) {	
    S += Pi[LigneDeLaBaseFactorisee[IndicesDeLigneDesTermesDuProblemeReduit[il]]] * ACol[il];
    il++;
  }
  CBarre[Var] = C[Var] - S;	
}

Spx->CBarreAEteCalculeParMiseAJour = NON_SPX;

return;
}

/*----------------------------------------------------------------------------*/
