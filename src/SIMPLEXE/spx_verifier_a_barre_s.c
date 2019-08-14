// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Verification de A_BARRE_S = B-1 * AS
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h" 

# define CYCLE_DE_VERIFICATION_DE_ABARRES              10  
# define CYCLE_DE_VERIFICATION_DE_ABARRES_HYPER_CREUX  20

/*----------------------------------------------------------------------------*/
/*            Verification du calcul de ABarreS pour savoir s'il faut
              refactoriser la base: on verifie sur k contraintes tirees
              au hasard  		         			      */

void SPX_VerifierABarreS( PROBLEME_SPX * Spx )
{

if ( Spx->TypeDeStockageDeABarreS == COMPACT_SPX ) {
  if ( Spx->Iteration % CYCLE_DE_VERIFICATION_DE_ABARRES_HYPER_CREUX != 0 ) return;
}
else {
  if ( Spx->Iteration % CYCLE_DE_VERIFICATION_DE_ABARRES != 0 ) return;
}

/*		   
printf("Verification de ABarreS dans SPX_VerifierABarreS iteration %d\n",Spx->Iteration); 
*/

if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
  SPX_VerifierABarreSAvecBaseReduite( Spx );
}
else {
  SPX_VerifierABarreSAvecBaseComplete( Spx );
}

return;
}

