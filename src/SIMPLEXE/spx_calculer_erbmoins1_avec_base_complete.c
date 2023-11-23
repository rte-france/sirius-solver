// Copyright (C) 2007-2022, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/***********************************************************************

   FONCTION: Resolution du systeme transpose pour obtenir une ligne
             de l'inverse de la base    

   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

# include "lu_define.h"

/*----------------------------------------------------------------------------*/
/* Calcul de la ligne de B^{-1} de la variable sortante */
void SPX_CalculerErBMoins1AvecBaseComplete( PROBLEME_SPX * Spx, char CalculEnHyperCreux ) 
{
char TypeDEntree; char TypeDeSortie; int i; double * ErBMoinsUn;

if ( CalculEnHyperCreux != OUI_SPX ) {
  TypeDEntree = VECTEUR_LU;	
  TypeDeSortie = VECTEUR_LU;
  /*memset( (char *) Spx->ErBMoinsUn , 0 , Spx->NombreDeContraintes * sizeof( double ) );*/
	ErBMoinsUn = Spx->ErBMoinsUn;
	for ( i = 0 ; i < Spx->NombreDeContraintes ; i++ ) ErBMoinsUn[i] = 0.0;
	
  Spx->ErBMoinsUn[Spx->ContrainteDeLaVariableEnBase[Spx->VariableSortante]] = 1.;
}
else {
  TypeDEntree  = COMPACT_LU;
	TypeDeSortie = COMPACT_LU;
  Spx->ErBMoinsUn[0] = 1;
  Spx->IndexTermesNonNulsDeErBMoinsUn[0] = Spx->ContrainteDeLaVariableEnBase[Spx->VariableSortante];
  Spx->NbTermesNonNulsDeErBMoinsUn = 1;
}

SPX_ResoudreUBEgalC( Spx, TypeDEntree, Spx->ErBMoinsUn, Spx->IndexTermesNonNulsDeErBMoinsUn,
										 &(Spx->NbTermesNonNulsDeErBMoinsUn), &TypeDeSortie, CalculEnHyperCreux );

if ( CalculEnHyperCreux == OUI_SPX ) {
  if ( TypeDeSortie != TypeDEntree ) {
    /* Ca s'est pas bien passe et on s'est forcement retrouve en VECTEUR_LU */
    Spx->TypeDeStockageDeErBMoinsUn = VECTEUR_SPX;		
		Spx->NbEchecsErBMoins++;
		/*
		printf("SPX_CalculerErBMoins1 echec hyper creux ErBMoins1 iteration %d\n",Spx->Iteration);
		*/
		if ( Spx->NbEchecsErBMoins >= SEUIL_ECHEC_CREUX ) {
      # if VERBOSE_SPX
		    printf("Arret de l'hyper creux pour le calcul de la ligne pivot, iteration %d\n",Spx->Iteration);
      # endif			
		  Spx->CalculErBMoinsUnEnHyperCreux = NON_SPX;
      Spx->CountEchecsErBMoins = 0;
		}		
	}
	else Spx->NbEchecsErBMoins = 0;
}
										 
return;
}
