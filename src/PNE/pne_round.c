// Copyright (C) 2007-2022, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/***********************************************************************

   FONCTION: Pour r�duire la pr�cision des coefficients utilis�s dans les calculs 
                
   AUTEUR: N. Lhuiller

************************************************************************/

# include "pne_sys.h"  
# include "pne_fonctions.h"

/* Pour r�duire la pr�cision des coefficients utilis�s dans les calculs */
double PNE_Round(double x, double prec) {

       if (x > 0)
             x = floor(x * prec + 0.5) / prec;
       else if (x < 0)
             x = ceil(x * prec - 0.5) / prec;
						 
       return x;
}
