// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Translater les bornes en sortie pour tout remettre     
             en ordre
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

/*----------------------------------------------------------------------------*/

void SPX_SauvegardesBranchAndBoundAndCut( PROBLEME_SPX * Spx )
{
	int Cnt; int Var; /*int i;*/ 

/* Sauvegardes pour le strong branching */

for ( Var = 0 ; Var < Spx->NombreDeVariables ; Var++ ) {
  Spx->XSV                           [Var] = Spx->X[Var];
  Spx->PositionDeLaVariableSV        [Var] = Spx->PositionDeLaVariable[Var];
  Spx->CBarreSV                      [Var] = Spx->CBarre[Var];    
  Spx->InDualFrameworkSV             [Var] = Spx->InDualFramework[Var];  
  Spx->ContrainteDeLaVariableEnBaseSV[Var] = Spx->ContrainteDeLaVariableEnBase[Var];
}

for ( Cnt = 0 ; Cnt < Spx->NombreDeContraintes ; Cnt++ ) {   
  Spx->BBarreSV                      [Cnt] = Spx->BBarre[Cnt]; 
  Spx->DualPoidsSV                   [Cnt] = Spx->DualPoids[Cnt]; 
  Spx->VariableEnBaseDeLaContrainteSV[Cnt] = Spx->VariableEnBaseDeLaContrainte[Cnt];
  Spx->CdebBaseSV                    [Cnt] = Spx->CdebBase[Cnt];
  Spx->NbTermesDesColonnesDeLaBaseSV [Cnt] = Spx->NbTermesDesColonnesDeLaBase[Cnt]; 
}

return;
}



















