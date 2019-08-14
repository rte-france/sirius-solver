// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Calcul du critere.
                            
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "pne_sys.h"
							     
# include "pne_fonctions.h"
# include "pne_define.h"

# include "spx_define.h"
# include "spx_fonctions.h"

/*----------------------------------------------------------------------------*/

void PNE_CalculerLaValeurDuCritere( PROBLEME_PNE * Pne )
{
int i ; 

/* Calcul du critere */
Pne->Critere = Pne->Z0;

for ( i = 0 ; i < Pne->NombreDeVariablesTrav ; i++ ) Pne->Critere+= Pne->LTrav[i] * Pne->UTrav[i];
 
if (Pne->pne_params->VERBOSE_PNE) {
	printf(" Valeur du critere apres optimisation du probleme relaxe: %lf \n", Pne->Critere);
	printf(" ********************************************************     \n");
}

if ( Pne->NombreDeCoupesCalculees == 0 ) Pne->ValeurOptimaleDuProblemeCourantAvantNouvellesCoupes = Pne->Critere; 

return;
}

