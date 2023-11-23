// Copyright (C) 2007-2022, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/***********************************************************************

   FONCTION: Resolution du systeme transpose pour obtenir une ligne
             de l'inverse de la base    

   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"   

# include "spx_fonctions.h"
# include "spx_define.h"

# include "lu_define.h"

/*----------------------------------------------------------------------------*/
/* Calcul de la ligne de B^{-1} de la variable sortante */
void SPX_CalculerErBMoins1( PROBLEME_SPX * Spx, char CalculEnHyperCreux ) 
{

if ( Spx->UtiliserLaBaseReduite == OUI_SPX ) {
  SPX_CalculerErBMoins1AvecBaseReduite( Spx, CalculEnHyperCreux );
}
else {
  SPX_CalculerErBMoins1AvecBaseComplete( Spx, CalculEnHyperCreux );
}
										 
return;
}
