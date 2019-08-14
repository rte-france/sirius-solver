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

void SPX_CalculerPiAvecBaseComplete( PROBLEME_SPX * Spx )
{
int Cnt; double * Pi; double * C; int * VariableEnBaseDeLaContrainte;
char TypeDEntree; char TypeDeSortie; char CalculEnHyperCreux; 

Pi = Spx->Pi;
C  = Spx->C;
VariableEnBaseDeLaContrainte = Spx->VariableEnBaseDeLaContrainte;

/* Boucle sur les variables en base */
for ( Cnt = 0 ; Cnt < Spx->NombreDeContraintes ; Cnt++ ) {
  Pi[Cnt] = C[VariableEnBaseDeLaContrainte[Cnt]];
}

TypeDEntree  = VECTEUR_LU;
TypeDeSortie = VECTEUR_LU;
CalculEnHyperCreux = NON_SPX;
SPX_ResoudreUBEgalC( Spx, TypeDEntree, Pi, NULL, NULL, NULL, CalculEnHyperCreux  );

return;      
}

/*----------------------------------------------------------------------------*/

