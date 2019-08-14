// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Pour réduire la précision des coefficients utilisés dans les calculs 
                
   AUTEUR: N. Lhuiller

************************************************************************/

# include "pne_sys.h"  
# include "pne_fonctions.h"

/* Pour réduire la précision des coefficients utilisés dans les calculs */
double PNE_Round(double x, double prec) {

       if (x > 0)
             x = floor(x * prec + 0.5) / prec;
       else if (x < 0)
             x = ceil(x * prec - 0.5) / prec;
						 
       return x;
}
