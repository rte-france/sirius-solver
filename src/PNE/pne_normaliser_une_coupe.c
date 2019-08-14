// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Normalisation d'une coupe
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "pne_sys.h"

# include "pne_fonctions.h"
# include "pne_define.h"

# include "spx_define.h"
# include "spx_fonctions.h"     

# include "bb_define.h"
# include "bb_fonctions.h"

# ifdef PNE_UTILISER_LES_OUTILS_DE_GESTION_MEMOIRE_PROPRIETAIRE	
  # include "pne_memoire.h"
# endif

/*----------------------------------------------------------------------------*/

void PNE_NormaliserUnCoupe( double * Coefficient, double * SecondMembre, int NombreDeTermes, double PlusGrandCoeff )
{
int i; double Normalisation; double Sec; 

Normalisation = 1.0 / PlusGrandCoeff;  
SPX_ArrondiEnPuissanceDe2( &Normalisation );  

for ( i = 0 ; i < NombreDeTermes ; i++ ) Coefficient[i] *= Normalisation;          
Sec = *SecondMembre;
Sec*= Normalisation;
*SecondMembre = Sec;
   
return;
}

