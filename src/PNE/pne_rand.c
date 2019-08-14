// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Fonction de tirage d'un nombre pseudo aleatoire entre 
             0 et 1
                  
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "pne_sys.h"

# include "pne_fonctions.h"
   
# define PI            3.141592653
# define PNE_RAND_MAX  1.

/********************************************************************/

double PNE_SRand( double Graine )
{
double A1;

A1 = Graine;
printf("Graine : %f\n", Graine);
return( PNE_Rand( A1 ) );

}

/********************************************************************/

double PNE_Rand( double A1 )
{
                                                                   
A1 = A1 + PI;                                                      
A1 = pow( A1, 5. );                                                          
A1 = A1 - floor( A1 );                                                    
A1 = A1 * PNE_RAND_MAX;    

if      ( A1 > PNE_RAND_MAX ) A1 = PNE_RAND_MAX;
else if ( A1 < 0.           ) A1 = 0.;

return( A1 );

}


