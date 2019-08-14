// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

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
