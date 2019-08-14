// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Translater les bornes en entree pour mettre le   
             probleme sous la forme standard 
               
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

/*----------------------------------------------------------------------------*/

void SPX_TranslaterLesBornes( PROBLEME_SPX * Spx )
{
int i; int Var; int Cnt; int il; int ilMax; double S; double * XminEntree;
double * XmaxEntree; double * Xmin; double * Xmax; char * TypeDeVariable;
double * XEntree; int * Mdeb; int * NbTerm; double * X; double * A;
double * B; int * Indcol; 

XminEntree     = Spx->XminEntree;
XmaxEntree     = Spx->XmaxEntree;
Xmin           = Spx->Xmin;
Xmax           = Spx->Xmax;
TypeDeVariable = Spx->TypeDeVariable;
XEntree        = Spx->XEntree;
X              = Spx->X;

for ( i = 0 ; i < Spx->NombreDeVariables ; i++ ) {  

  XminEntree[i] = Xmin[i];
  XmaxEntree[i] = Xmax[i]; 

  if ( TypeDeVariable[i] == NON_BORNEE ) continue;

  if ( TypeDeVariable[i] == BORNEE ) { 
    Xmax[i] = XmaxEntree[i] - XminEntree[i];
  }
  Xmin[i] = 0.;

  XEntree[i] = X[i];
	
  if ( TypeDeVariable[i] == BORNEE || TypeDeVariable[i] == BORNEE_INFERIEUREMENT ) { 	
    X[i]= XEntree[i] - XminEntree[i];
  }
	
}


Mdeb   = Spx->Mdeb;
NbTerm = Spx->NbTerm;
Indcol = Spx->Indcol;
A      = Spx->A;
B      = Spx->B;

for ( Cnt = 0 ; Cnt < Spx->NombreDeContraintes ; Cnt++ ) {
  il    = Mdeb[Cnt];
  ilMax = il + NbTerm[Cnt];
  S     = 0.;
  while ( il < ilMax ) {
    Var = Indcol[il];
    if ( TypeDeVariable[Var] != NON_BORNEE ) S+= A[il] * XminEntree[Var];
    il++;
  }
  B[Cnt]-=S;
} 

return;
}
