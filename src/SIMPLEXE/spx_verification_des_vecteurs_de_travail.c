// Copyright (C) 2019, RTE (http://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0
/***********************************************************************

   FONCTION: Verification de vecteurs de travail. Utilise en mode debug
	           pour verifier qu'on a bien remis la bonne valeur dans tous les
						 vecteurs de travail. 
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"  

# include "spx_fonctions.h"
# include "spx_define.h"

/*----------------------------------------------------------------------------*/

void SPX_VerifierLesVecteursDeTravail( PROBLEME_SPX * Spx )
{
int i;

for ( i = 0 ; i < Spx->NombreDeVariables ; i++ ) {
  if ( Spx->T[i] != -1 ) {
	  printf("SPX_VerifierLesVecteursDeTravail: T incorrect\n");
		exit(0);
	}
}
for ( i = 0 ; i < Spx->NombreDeContraintes ; i++ ) {
  if ( Spx->Marqueur[i] != -1 ) {
	  printf("SPX_VerifierLesVecteursDeTravail: Marqueur incorrect Marqueur[%d] = %d\n",i,Spx->Marqueur[i]);
		exit(0);
	}	
  if ( Spx->AReduit[i] != 0 ) {
	  printf("SPX_VerifierLesVecteursDeTravail: AReduit incorrect AReduit[%d] = %e\n",i,Spx->AReduit[i]);
		exit(0);
	}	
  if ( Spx->Tau[i] != 0 ) {
    printf("SPX_VerifierLesVecteursDeTravail: Tau incorrect Tau[%d] = %e\n",i,Spx->Tau[i]);
    exit(0);
  }	
}

return;
}
