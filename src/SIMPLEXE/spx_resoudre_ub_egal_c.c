// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION:   Calcul de pi = Cb * B_MOINS_1 c'est a dire              
               resolution de u B = c

   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

# include "lu_define.h"

/*----------------------------------------------------------------------------*/
												
void SPX_ResoudreUBEgalC( PROBLEME_SPX * Spx,
                          char TypeDEntree, /* Mode de stockage du vecteur second membre */
                          double * U, /* Second membre et resultat */
                          int * IndexDesTermesNonNuls,
													int * NombreDeTermesNonNuls,
													char * TypeDeSortie, 
                          char CalculEnHyperCreux /* Vaut OUI_SPX ou NON_SPX */
												)
{ 
char SecondMembreCreux;     

if ( Spx->UtiliserLaLuUpdate == NON_SPX ) {
  SPX_AppliquerLesEtaVecteursTransposee( Spx, U, IndexDesTermesNonNuls, NombreDeTermesNonNuls,
	                                       CalculEnHyperCreux, TypeDEntree );	
}

/* Terminer par la resolution avec la derniere base factorisee */

SecondMembreCreux = NON_LU;
SPX_ResolutionDeSystemeTransposee( Spx, TypeDEntree, U, IndexDesTermesNonNuls, NombreDeTermesNonNuls,																		
                                   TypeDeSortie, CalculEnHyperCreux, SecondMembreCreux );  
																	 
return;
}
