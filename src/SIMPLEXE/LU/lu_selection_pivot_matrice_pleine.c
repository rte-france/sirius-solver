// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Decomposition LU de la base. Methode de pivot total.
                
 
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "lu_define.h"
# include "lu_fonctions.h"

# if OUTILS_DE_GESTION_MEMOIRE_DE_PNE_SOLVEUR	
  # include "mem_fonctions.h"
#endif

/*--------------------------------------------------------------*/
/*                   Selection du pivot                         */
/* Comme on est en matrice pleine on prend le plus grand terme  */
/* de la sous matrice active                                    */

void LU_SelectionDuPivotMatricePleine( MATRICE * Matrice,
                                       int * LignePivotChoisie, 
                                       int * ColonnePivotChoisie 
                                     )
{
/* Remarque, ça marche aussi dans le cas du pivotage diagonal car
   dans ce cas le plus grand terme est recherche sur la diagonale */
if ( Matrice->ValeurDuPlusGrandTerme > Matrice->PivotMin ) {
  *LignePivotChoisie   = Matrice->LigneDuPlusGrandTerme;
  *ColonnePivotChoisie = Matrice->ColonneDuPlusGrandTerme;

	
}
else {
  *LignePivotChoisie   = -1;
  *ColonnePivotChoisie = -1;
}
if ( *ColonnePivotChoisie >= 0 ) {
  if ( Matrice->CNbTerm[*ColonnePivotChoisie] != Matrice->CNbTermMatriceActive[*ColonnePivotChoisie] ) {
    LU_SupprimerTermesInutilesDansColonne( Matrice, *ColonnePivotChoisie, Matrice->Cdeb[*ColonnePivotChoisie] );
  }
}
return;
}


/*--------------------------------------------------------------*/
/*   Recherche du plus grand terme de la matrice active juste   */
/*   avant le passage en mode matrice pleine                    */

void  LU_RechercherLePlusGrandTermeDeLaMatriceActive( MATRICE * Matrice )
{
int Kp; int Ligne; int il; double X; int NombreDeTermes; int ilDeb;
int * Ldeb; int * LNbTerm; int * LIndiceColonne; double * Elm;

Ldeb = Matrice->Ldeb;
LNbTerm = Matrice->LNbTerm;
LIndiceColonne = Matrice->LIndiceColonne;
Elm = Matrice->Elm;

Matrice->ValeurDuPlusGrandTerme = -1.;
if ( Matrice->FaireDuPivotageDiagonal == NON_LU ) {
  for ( Kp = Matrice->Kp + 1 ; Kp < Matrice->Rang ; Kp++ ) {
    Ligne = Matrice->OrdreLigne[Kp];
		ilDeb          = Ldeb[Ligne];
    NombreDeTermes = LNbTerm[Ligne];
    for ( il = ilDeb ; il < ilDeb + NombreDeTermes ; il++ ) { 
      X = fabs( Elm[il] );
      if ( X > Matrice->ValeurDuPlusGrandTerme ) {
        Matrice->ValeurDuPlusGrandTerme  = X;
        Matrice->LigneDuPlusGrandTerme   = Ligne;
        Matrice->ColonneDuPlusGrandTerme = LIndiceColonne[il];
      }
    }
  }
}
else {
  /* Pivotage diagonal */  
  for ( Kp = Matrice->Kp + 1 ; Kp < Matrice->Rang ; Kp++ ) {
    Ligne = Matrice->OrdreLigne[Kp];    		
		ilDeb          = Ldeb[Ligne];
    NombreDeTermes = LNbTerm[Ligne];		
    for ( il = ilDeb ; il < ilDeb + NombreDeTermes ; il++ ) {     
      /* Recherche du terme diagonal */
      if ( LIndiceColonne[il] == Ligne ) {
        X = fabs( Elm[il] );   
        if ( X > Matrice->ValeurDuPlusGrandTerme ) {
          Matrice->ValeurDuPlusGrandTerme  = X;
          Matrice->LigneDuPlusGrandTerme   = Ligne;
          Matrice->ColonneDuPlusGrandTerme = Ligne;
        }
        break;
      }
    }
  }
}

return;
}





























































