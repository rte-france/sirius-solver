// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Decomposition LU de la base.
             Scan ligne dans le cas ou la matrice est pleine.
                
 
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "lu_define.h"
# include "lu_fonctions.h"

# if OUTILS_DE_GESTION_MEMOIRE_DE_PNE_SOLVEUR	
  # include "mem_fonctions.h"
#endif

/*--------------------------------------------------------------------------------------------------*/
/* Les termes de la ligne pivot sont compares a ceux de la ligne scannee et les nouvelle valeurs 
   des termes de la ligne scannee sont calculees */

void LU_ScanLigneMatricePleine ( MATRICE * Matrice,
                                 int   Ligne, 
                                 int   ColonnePivot,  
                                 double ValTermeColonnePivot
                               ) 
{
int il; int ilDeb; int Colonne; double X; double Y; int NbTermesLigne;
int * LIndiceColonne; double * Elm;

/* Transformation des termes de la ligne scannee */

ilDeb          = Matrice->Ldeb[Ligne];
NbTermesLigne  = Matrice->LNbTerm[Ligne];
LIndiceColonne = Matrice->LIndiceColonne;
Elm            = Matrice->Elm;

if ( Matrice->FaireDuPivotageDiagonal == NON_LU ) {
  for ( il = ilDeb ; il < ilDeb + NbTermesLigne ; il++ ) {
    Colonne = LIndiceColonne[il];
    /* Calcul du terme modifie */
    /* W[ColonnePivot] a ete force a 0 pour eviter le test "IndiceColonne ? ColonnePivot" */
    X = Elm[il] - ( Matrice->W[Colonne] * ValTermeColonnePivot );
    Elm[il] = X;    
    Y = fabs( X );    
    if ( Y > Matrice->ValeurDuPlusGrandTerme ) {
      if ( Colonne != ColonnePivot ) {
        Matrice->ValeurDuPlusGrandTerme  = Y;
        Matrice->LigneDuPlusGrandTerme   = Ligne;
        Matrice->ColonneDuPlusGrandTerme = Colonne;
      }
    }  
  }
}
else {
  /* Cas du pivotage diagonal */
  for ( il = ilDeb ; il < ilDeb + NbTermesLigne ; il++ ) { 
    Colonne = LIndiceColonne[il];
    /* Calcul du terme modifie */
    /* W[ColonnePivot] a ete force a 0 pour eviter le test "IndiceColonne ? ColonnePivot" */
    X = Elm[il] - ( Matrice->W[Colonne] * ValTermeColonnePivot );
    Elm[il] = X;
    if ( Colonne == Ligne ) {    
      Y = fabs( X );    
      if ( Y > Matrice->ValeurDuPlusGrandTerme ) {
        if ( Colonne != ColonnePivot ) {
          Matrice->ValeurDuPlusGrandTerme  = Y;
          Matrice->LigneDuPlusGrandTerme   = Ligne;
          Matrice->ColonneDuPlusGrandTerme = Ligne;
        }
      }
    }  
  }  
}

return;
}






























































