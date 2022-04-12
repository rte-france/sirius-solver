// Copyright (C) 2007-2018, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/***********************************************************************

   FONCTION:   Calcul de Pi = c_B * B^{-1} c'est a dire              
               resolution de u B = c

   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

# include "lu_define.h"

/*----------------------------------------------------------------------------*/

void SPX_CalculerPi( PROBLEME_SPX * Spx )
{

if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
  SPX_CalculerPiAvecBaseReduite( Spx );
}
else {
  SPX_CalculerPiAvecBaseComplete( Spx );
}

return;  
}

/*----------------------------------------------------------------------------*/
/*                        Mise a jour de Pi                                   */
							       
void SPX_MettreAJourPi( /* PROBLEME_SPX * Spx */ )
{
/* A faire dans le futur car cela n'a pas grand interet */

return;
}
