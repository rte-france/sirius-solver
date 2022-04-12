// Copyright (C) 2007-2018, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/***********************************************************************

   FONCTION: Calcul de BBarre = B^{-1} * b 

                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"  

# include "spx_fonctions.h"
# include "spx_define.h"

# include "lu_define.h"

/*----------------------------------------------------------------------------*/

void SPX_CalculerBBarre( PROBLEME_SPX * Spx )
{

if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
  SPX_CalculerBBarreAvecBaseReduite( Spx );
}
else {
  SPX_CalculerBBarreAvecBaseComplete( Spx );
}

Spx->BBarreAEteCalculeParMiseAJour = NON_SPX;

SPX_InitialiserLesVariablesEnBaseAControler( Spx );

return;
}

