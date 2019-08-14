// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Gestion des temps de calcul
               
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"


/*----------------------------------------------------------------------------*/

void SPX_InitDateDebutDuCalcul( PROBLEME_SPX * Spx )
{
time_t HeureDeCalendrierDebut;

time( &HeureDeCalendrierDebut );
Spx->HeureDeCalendrierDebut = HeureDeCalendrierDebut;

return;
}

/*----------------------------------------------------------------------------*/

void SPX_ControleDuTempsEcoule( PROBLEME_SPX * Spx )
{
time_t HeureDeCalendrierCourant;
double TempsEcoule;

if ( Spx->DureeMaxDuCalcul < 0 ) return;

time( &HeureDeCalendrierCourant );

TempsEcoule = difftime( HeureDeCalendrierCourant , Spx->HeureDeCalendrierDebut );

if ( TempsEcoule <= 0.0 ) TempsEcoule = 0.0;

/* On provoque l'arret du calcul si temps depasse */
if ( TempsEcoule > Spx->DureeMaxDuCalcul ) Spx->Iteration = 10 * Spx->NombreMaxDIterations;

return;
}
