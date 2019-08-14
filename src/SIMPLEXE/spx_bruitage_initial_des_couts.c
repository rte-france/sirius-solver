// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Bruitage intial des couts
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

# include "pne_fonctions.h"

/*----------------------------------------------------------------------------*/

void SPX_BruitageInitialDesCouts( PROBLEME_SPX * Spx )
{
int Var; double Random; double RandomMax; double RandomMin; double * C; char * TypeDeVariable;
double * SeuilDAmissibiliteDuale; double CoutMoyen; int NbCoutsNonNuls; double RdSup;
double EcartMoyen; double * Xmin; double * Xmax; 

C = Spx->C;
TypeDeVariable = Spx->TypeDeVariable;
Xmin = Spx->Xmin;
Xmax = Spx->Xmax;
CoutMoyen = 0;
EcartMoyen = 0;
NbCoutsNonNuls = 0;
Spx->CoutMoyen = 0;
Spx->EcartDeBornesMoyen = 0;
Spx->PerturbationMax = 10;
/* Calcul du cout moyen sur les couts non nuls */
for ( Var = 0 ; Var < Spx->NombreDeVariablesNatives ; Var++ ) {
  if ( TypeDeVariable[Var] == NON_BORNEE ) continue;
  if ( C[Var] != 0 ) {
    CoutMoyen += fabs( C[Var] );  
		NbCoutsNonNuls++;
		if ( TypeDeVariable[Var] == BORNEE ) EcartMoyen += Xmax[Var] - Xmin[Var];
		else EcartMoyen += 1000;
	}
}
if ( NbCoutsNonNuls != 0 ) {
  CoutMoyen /= NbCoutsNonNuls;
  Spx->CoutMoyen = CoutMoyen;
  EcartMoyen /= NbCoutsNonNuls;
  Spx->EcartDeBornesMoyen = EcartMoyen;
	Spx->PerturbationMax = 0.0001 * Spx->CoutMoyen * Spx->EcartDeBornesMoyen;
}

if (Spx->spx_params->FAIRE_UN_BRUITAGE_INITIAL_DES_COUTS != OUI_SPX) {
	return;
}

SeuilDAmissibiliteDuale = Spx->SeuilDAmissibiliteDuale1;

Spx->LesCoutsOntEteModifies = OUI_SPX;

RdSup = Spx->spx_params->SEUIL_ADMISSIBILITE_DUALE_2;

for ( Var = 0 ; Var < Spx->NombreDeVariablesNatives ; Var++ ) {
	if ( C[Var] == 0 ) continue;
  if ( TypeDeVariable[Var] == NON_BORNEE ) continue;
  Spx->A1 = PNE_Rand( Spx->A1 ); /* Nombre aleatoire entre 0 et 1 */
	
  RandomMax = SeuilDAmissibiliteDuale[Var];	
	if ( RandomMax > RdSup ) RandomMax = RdSup;
	
  RandomMin = -RandomMax;
	
  Random = RandomMin;
  Random += Spx->A1 * ( RandomMax - RandomMin );
	
	C[Var] += Random;
}

return;
}
