// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Resoudre B y = a en utilisant la forme produit de 
             l'inverse si necessaire.
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

/*----------------------------------------------------------------------------*/

void SPX_ResoudreBYegalA( PROBLEME_SPX * Spx,
                          char TypeDEntree, /* Mode de stockage du vecteur second membre */
                          double * A,       /* Second membre et solution */
                          int * IndexDesTermesNonNuls,
													int * NombreDeTermesNonNuls,
													char * TypeDeSortie, /* Mode de stockage demande pour la solution */
													
                          char CalculEnHyperCreux, /* Vaut OUI_SPX ou NON_SPX */
                          char Save,  /* Sauvegarde ou non du resultat de la triangulaire inferieure */
			                    char SecondMembreCreux /* Vaut OUI_LU ou NON_LU */
                        )
{
/* Il faut d'abord resoudre B y = a en prenant pour B la derniere base factorisee
   et eventuellement appliquer les eta vecteurs dans le cas de la forme produit de
	 l'inverse */

SPX_ResolutionDeSysteme( Spx, TypeDEntree, A, IndexDesTermesNonNuls, NombreDeTermesNonNuls,
												 TypeDeSortie, CalculEnHyperCreux, Save, SecondMembreCreux );
   
/* Si necessaire, on applique les eta vecteurs (cas: forme produit de l'inverse) */
if ( Spx->UtiliserLaLuUpdate == OUI_SPX ) return;

SPX_AppliquerLesEtaVecteurs( Spx, A, IndexDesTermesNonNuls, NombreDeTermesNonNuls, CalculEnHyperCreux, *TypeDeSortie );

/* On ne cherche pas a passer en mode plein apres application des eta vecteurs */

return;
}
  
  
