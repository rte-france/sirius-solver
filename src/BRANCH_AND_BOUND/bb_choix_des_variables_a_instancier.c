// Copyright (C) 2007-2022, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/***********************************************************************

   FONCTION: Apres la resolution d'un probleme relaxe, ce sous-programme 
   initialise le numero de la prochaine variable a instancier dans les 
   2 noeuds fils.
         
   AUTEUR: R. GONZALEZ

************************************************************************/

#include "bb_sys.h"
#include "bb_define.h"
#include "bb_fonctions.h"

#include "pne_fonctions.h"

/*---------------------------------------------------------------------------------------------------------*/

int BB_ChoixDesVariablesAInstancier( BB * Bb,
				      NOEUD *  NoeudCourant,
                                      int *   ValeurDInstanciationAGauche,
                                      int *   NombreDeVariablesAInstancierAGauche,
                                      int **  NumerosDesVariablesAInstancierAGauche,
				      int *   ValeurDInstanciationADroite,
                                      int *   NombreDeVariablesAInstancierADroite,
                                      int **  NumerosDesVariablesAInstancierADroite
                                    )
{
int CodeRetour; PROBLEME_PNE * Pne;

Pne = (PROBLEME_PNE *) Bb->ProblemePneDuBb;

CodeRetour = PAS_DE_VARIABLES_A_INSTANCIER;

if( NoeudCourant->LaSolutionRelaxeeEstEntiere == OUI ) {
  CodeRetour = PAS_DE_VARIABLES_A_INSTANCIER;
  return( CodeRetour );
}
 
if( NoeudCourant->NoeudTerminal == OUI ) {
  CodeRetour = PAS_DE_VARIABLES_A_INSTANCIER;
  return( CodeRetour );
}
   
/* Si le noeud est A_REJETER, il n'y a rien a instancier */
if ( NoeudCourant->StatutDuNoeud == A_REJETER ) {
  CodeRetour = PAS_DE_VARIABLES_A_INSTANCIER;
  return( CodeRetour );
}

/* Appel de la partie PNE: on recupere soit une variable, soit une Gub */
PNE_ChoixDesVariablesAInstancier( Pne,
                                  ValeurDInstanciationAGauche,
                                  NombreDeVariablesAInstancierAGauche,
                                  NumerosDesVariablesAInstancierAGauche,
				                          ValeurDInstanciationADroite,
                                  NombreDeVariablesAInstancierADroite,
                                  NumerosDesVariablesAInstancierADroite
				);
						
CodeRetour = VARIABLES_A_INSTANCIER;
return( CodeRetour );

}

