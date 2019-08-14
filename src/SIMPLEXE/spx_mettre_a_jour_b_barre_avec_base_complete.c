// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Mise a jour de BBarre = B^{-1} * b 

                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"  

# include "spx_fonctions.h"
# include "spx_define.h"

# include "lu_define.h"
# include "lu_fonctions.h"

void SPX_CalculerBsHyperCreuxAvecBaseComplete( PROBLEME_SPX * , char * , int * , int * );
void SPX_CalculerBsStandardAvecBaseComplete( PROBLEME_SPX * , char * );

/*----------------------------------------------------------------------------*/
/* On tente un calcul en hyper creux s'il y a des chances de succes */
void SPX_CalculerBsHyperCreuxAvecBaseComplete( PROBLEME_SPX * Spx, char * ResoudreLeSysteme, 
                                               int * IndexTermesNonNuls, int * NbTNonNuls )
{
int il; int ilMax; int i; int Var; double Xmx; char Flag; double * Bs; int * T;
int * BoundFlip; double * Xmax; int * NumeroDeContrainte; double * ACol; int * Cdeb;
int * CNbTerm; int NbTermesNonNuls; int k;

T = Spx->T;
NbTermesNonNuls = 0;	
Bs        = Spx->Bs;
BoundFlip = Spx->BoundFlip;
Xmax      = Spx->Xmax;

Cdeb               = Spx->Cdeb;
CNbTerm            = Spx->CNbTerm;
NumeroDeContrainte = Spx->NumeroDeContrainte;
ACol               = Spx->ACol;

Flag = NON_SPX;
for ( i = 0 ; i < Spx->NbBoundFlip; i++ ) { 
  Var = BoundFlip[i];
	if ( BoundFlip[i] > 0 ) {
	  Var = BoundFlip[i] - 1;
		Xmx = Xmax[Var];	
	}
	else {
	  Var = -BoundFlip[i] - 1;
		Xmx = -Xmax[Var];	
	}
  if ( Xmx == 0.0 ) continue;
  Flag  = OUI_SPX;
  il    = Cdeb[Var];
  ilMax = il + CNbTerm[Var];
  while ( il < ilMax ) {
	  k = NumeroDeContrainte[il];
		if ( T[k] == -1 ) {
      T[k] = 1;
			IndexTermesNonNuls[NbTermesNonNuls] = k;
			NbTermesNonNuls++;				
		}			
    Bs[k]+= Xmx * ACol[il];		  
    il++;
  }
}

/* RAZ de T */
for ( i = 0 ; i < NbTermesNonNuls ; i++ ) T[IndexTermesNonNuls[i]] = -1;	 		

*ResoudreLeSysteme = Flag;
*NbTNonNuls        = NbTermesNonNuls;
return;
}

/*----------------------------------------------------------------------------*/
void SPX_CalculerBsStandardAvecBaseComplete( PROBLEME_SPX * Spx, char * ResoudreLeSysteme )
{

int il; int ilMax; int i; int Var; double Xmx; char Flag; double * Bs;
int * BoundFlip; double * Xmax; int * NumeroDeContrainte;
double * ACol; int * Cdeb; int * CNbTerm; 

Bs         = Spx->Bs;
BoundFlip  = Spx->BoundFlip;
Xmax       = Spx->Xmax;

Cdeb               = Spx->Cdeb;
CNbTerm            = Spx->CNbTerm;
NumeroDeContrainte = Spx->NumeroDeContrainte;
ACol               = Spx->ACol;

Flag = NON_SPX;
for ( i = 0 ; i < Spx->NbBoundFlip; i++ ) { 
  Var = BoundFlip[i];
	if ( BoundFlip[i] > 0 ) {
	  Var = BoundFlip[i] - 1;
		Xmx = Xmax[Var];	
	}
	else {
	  Var = -BoundFlip[i] - 1;
		Xmx = -Xmax[Var];	
	}
  if ( Xmx == 0.0 ) continue;
  Flag  = OUI_SPX;
  il    = Cdeb[Var];
  ilMax = il + CNbTerm[Var];
  while ( il < ilMax ) {
    Bs[NumeroDeContrainte[il]]+= Xmx * ACol[il];		
    il++;
  }
}
	
*ResoudreLeSysteme = Flag; 

return;
}

/*----------------------------------------------------------------------------*/
/*                    Cas de la mise a jour de BBarre                         */

void SPX_MettreAJourBBarreAvecBaseComplete( PROBLEME_SPX * Spx, double * Bs, int * IndexTermesNonNuls,
                                            int * NbTermesNonNuls, char * StockageDeBs )
{
char Save; char SecondMembreCreux; char ResoudreLeSysteme; char TypeDEntree; char TypeDeSortie; 
char ResolutionEnHyperCreux;
  
if ( Spx->CalculABarreSEnHyperCreux == OUI_SPX ) ResolutionEnHyperCreux = OUI_SPX;
else ResolutionEnHyperCreux = NON_SPX;  

if ( Spx->TypeDeCreuxDeLaBase == BASE_HYPER_CREUSE && ResolutionEnHyperCreux == OUI_SPX ) {
  SPX_CalculerBsHyperCreuxAvecBaseComplete( Spx, &ResoudreLeSysteme, IndexTermesNonNuls, NbTermesNonNuls );
	TypeDEntree = ADRESSAGE_INDIRECT_LU;
	TypeDeSortie = COMPACT_LU;      
	*StockageDeBs = COMPACT_SPX;	
}
else {
	ResolutionEnHyperCreux = NON_SPX;
  SPX_CalculerBsStandardAvecBaseComplete( Spx, &ResoudreLeSysteme );
	*StockageDeBs = VECTEUR_SPX;
	TypeDEntree = VECTEUR_LU;
	TypeDeSortie = VECTEUR_LU;      
}

/* Je pense qu'il y a un bug dans le type de stockage de StockageDeBs qu'on recupere
   car TypeDeSortie demande est soit VECTEUR soit COMPACT */

if ( ResoudreLeSysteme == OUI_SPX ) {
  Save = NON_LU;
  SecondMembreCreux = OUI_LU;
	
	if ( ResolutionEnHyperCreux == OUI_SPX ) {	
	  if ( *NbTermesNonNuls >= ceil(  Spx->spx_params->TAUX_DE_REMPLISSAGE_POUR_VECTEUR_HYPER_CREUX * Spx->NombreDeContraintes ) ) {
		  ResolutionEnHyperCreux = NON_SPX;
	    *StockageDeBs = VECTEUR_SPX;
	    TypeDEntree = VECTEUR_LU;
	    TypeDeSortie = VECTEUR_LU;      			
		}	 		
	}
	
  SPX_ResoudreBYegalA( Spx, TypeDEntree, Bs, IndexTermesNonNuls, NbTermesNonNuls, &TypeDeSortie,
                       ResolutionEnHyperCreux, Save, SecondMembreCreux );
											 
  if ( ResolutionEnHyperCreux == OUI_SPX ) {
    if ( TypeDeSortie != COMPACT_LU ) {		
      *StockageDeBs = VECTEUR_SPX;
    }		
  }
														 
}

return;
}   

