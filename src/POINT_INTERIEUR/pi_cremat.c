// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.


/***********************************************************************

   FONCTION: Creation de la matrice du point interieur.
        
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "pi_sys.h"

# include "pi_fonctions.h"
# include "pi_define.h"

# include "lu_constantes_externes.h"
# include "lu_definition_arguments.h"

# include "lu_define.h"
# include "lu_fonctions.h"

/*-------------------------------------------------------------------------*/
/*     Allocation de la matrice, calcul des termes, stockage des termes    */
 
void PI_Cremat( PROBLEME_PI * Pi )
{

PI_CrematSystemeAugmente( Pi );
		  
return;
}

/*-------------------------------------------------------------------------*/
/*  Calcul de termes de la matrice a chaque iteration puis factorisation   */

void PI_Crebis( PROBLEME_PI * Pi )
{

PI_CrebisSystemeAugmente( Pi );

return;
}




