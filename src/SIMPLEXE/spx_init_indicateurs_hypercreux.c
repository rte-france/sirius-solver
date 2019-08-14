// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

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
