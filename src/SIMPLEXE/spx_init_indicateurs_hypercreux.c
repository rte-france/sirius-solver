// Copyright (C) 2007-2022, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/***********************************************************************

   FONCTION: Initialisation des inticateurs hyper creux
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"
  
/*----------------------------------------------------------------------------*/

void SPX_InitialiserLesIndicateursHyperCreux( PROBLEME_SPX * Spx )
{

Spx->CalculErBMoinsUnEnHyperCreux       = OUI_SPX;
Spx->CalculErBMoinsEnHyperCreuxPossible = OUI_SPX;
Spx->CountEchecsErBMoins                = 0;                
Spx->AvertissementsEchecsErBMoins       = 0;      
Spx->NbEchecsErBMoins                   = 0;

Spx->CalculABarreSEnHyperCreux         = OUI_SPX;
Spx->CalculABarreSEnHyperCreuxPossible = OUI_SPX;
Spx->CountEchecsABarreS                = 0;                
Spx->AvertissementsEchecsABarreS       = 0;    
Spx->NbEchecsABarreS                   = 0;

return;
}
