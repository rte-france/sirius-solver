// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/*************************************************************************

   FONCTION: Evaluation des 2 fils du noeud comportant le meilleur
             minorant

   AUTEUR: R. GONZALEZ

**************************************************************************/

#include "bb_sys.h"
#include "bb_define.h"
#include "bb_fonctions.h"

#include "pne_define.h"

/*---------------------------------------------------------------------------------------------------------*/

void BB_EvaluerLesDeuxFilsDuMeilleurMinorant( BB * Bb, NOEUD * NoeudDuMeilleurMinorant )
{
int YaUneSolution; NOEUD * NoeudCourant; int SolutionEntiereTrouvee;  

#if VERBOSE_BB
  printf("Minorant du meilleur noeud %12.8e\n",NoeudDuMeilleurMinorant->MinorantDuCritereAuNoeud);
#endif
  
NoeudCourant = NoeudDuMeilleurMinorant->NoeudSuivantGauche;
if ( NoeudCourant != 0 ) {
  if ( NoeudCourant->StatutDuNoeud == A_EVALUER && 
       NoeudCourant->NoeudTerminal != OUI       && 
       NoeudCourant->StatutDuNoeud != A_REJETER ) {

    #if VERBOSE_BB
      printf("Evaluation fils gauche du meilleur minorant\n");
    #endif

    Bb->NoeudEnExamen = NoeudCourant;
    YaUneSolution = BB_ResoudreLeProblemeRelaxe( Bb, NoeudCourant , &SolutionEntiereTrouvee ); 

    BB_NettoyerLArbre( Bb, &YaUneSolution , NoeudCourant );  /* Fait aussi la mise a jour du statut */

    
    BB_CreerLesNoeudsFils( Bb, NoeudCourant );    

  }
}

NoeudCourant = NoeudDuMeilleurMinorant->NoeudSuivantDroit;
if ( NoeudCourant != 0 ) {
  if ( NoeudCourant->StatutDuNoeud == A_EVALUER && 
       NoeudCourant->NoeudTerminal != OUI       && 
       NoeudCourant->StatutDuNoeud != A_REJETER ) {

    #if VERBOSE_BB
      printf("Evaluation fils droit du meilleur minorant\n");
    #endif

    Bb->NoeudEnExamen = NoeudCourant;
    YaUneSolution = BB_ResoudreLeProblemeRelaxe( Bb, NoeudCourant , &SolutionEntiereTrouvee ); 
  
    BB_NettoyerLArbre( Bb, &YaUneSolution , NoeudCourant );  /* Fait aussi la mise a jour du statut */

    BB_CreerLesNoeudsFils( Bb, NoeudCourant );    

  }
}

return;
}

