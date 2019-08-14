// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Calcul des coupes d'intersection (necessite d'avoir
             calcule les gomory avant)
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "pne_sys.h"

# include "pne_fonctions.h"
# include "pne_define.h"

# include "spx_define.h"
# include "spx_fonctions.h"

/*----------------------------------------------------------------------------*/

void PNE_CalculerLesCoupesDIntersection( PROBLEME_PNE * Pne )
{
int i; int j; int NbPotentielDeCoupes; int NombreDeTermes; double S; double X;
double SecondMembre; double PlusGrandCoeff; char OnAEcrete; double Y; int NombreDeVariablesTrav;
double RapportMaxDesCoeffs; double ZeroPourCoeffVariablesDEcart; double ZeroPourCoeffVariablesNative;
double RelaxRhsAbs; double RelaxRhsRel; int * IndiceDeLaVariable; double * Coefficient;
double * UTrav;

RapportMaxDesCoeffs = Pne->pne_params->RAPPORT_MAX_COEFF_COUPE_INTERSECTION;
ZeroPourCoeffVariablesDEcart = Pne->pne_params->ZERO_POUR_COEFF_VARIABLE_DECART_DANS_COUPE_GOMORY_OU_INTERSECTION;
ZeroPourCoeffVariablesNative = Pne->pne_params->ZERO_POUR_COEFF_VARIABLE_NATIVE_DANS_COUPE_GOMORY_OU_INTERSECTION;
RelaxRhsAbs = Pne->pne_params->RELAX_RHS_INTERSECTION_ABS;
RelaxRhsRel = Pne->pne_params->RELAX_RHS_INTERSECTION_REL;

IndiceDeLaVariable = Pne->IndiceDeLaVariable_CG;
Coefficient = Pne->Coefficient_CG;
NombreDeVariablesTrav = Pne->NombreDeVariablesTrav;
UTrav = Pne->UTrav;

/* C'est la reduction des normes qui prend le plus de temps et de loin */
SPX_ReductionDesNormesPourCoupesDIntersection( (PROBLEME_SPX *) Pne->ProblemeSpxDuSolveur );

NbPotentielDeCoupes = SPX_NombrePotentielDeCoupesDIntersection( (PROBLEME_SPX *) Pne->ProblemeSpxDuSolveur );

/* NbPotentielDeCoupes ne joue pas sur le temps car SPX_CalculerUneCoupeDIntersection est tres rapide */

if (Pne->pne_params->CALCULER_COUPES_DINTERSECTION == NON_PNE) {
	SPX_TerminerLeCalculDesCoupesDIntersection((PROBLEME_SPX *)Pne->ProblemeSpxDuSolveur);
	return;
}

for ( i = 0 ; i < NbPotentielDeCoupes ; i++ ) {
  SPX_CalculerUneCoupeDIntersection(
              (PROBLEME_SPX *) Pne->ProblemeSpxDuSolveur,  
              i, 
			 
              RapportMaxDesCoeffs,
			        ZeroPourCoeffVariablesDEcart,
			        ZeroPourCoeffVariablesNative,
              RelaxRhsAbs,
              RelaxRhsRel,
			 							
              &NombreDeTermes,  
              Coefficient,  
              IndiceDeLaVariable,
              &SecondMembre,
							&OnAEcrete );
							
  if ( NombreDeTermes <= 0 ) continue;
  	    
  PlusGrandCoeff = -LINFINI_PNE;  
  S = 0.0;
  for ( j = 0 ; j < NombreDeTermes ; j++ ) {
    Y = Coefficient[j];
    if ( IndiceDeLaVariable[j] < NombreDeVariablesTrav ) {
      S += Y * UTrav[IndiceDeLaVariable[j]];
	    if ( fabs( Y ) > PlusGrandCoeff ) PlusGrandCoeff = fabs( Y );
    }   
  }
	
  if ( S > SecondMembre ) {  
    X = fabs( SecondMembre - S );	 
    if ( X > Pne->pne_params->SEUIL_VIOLATION_COUPE_DINTERSECTION ) {
		
		  /*
      printf("Coupe d'Intersection Violation %e NombreDeTermes %d (max.: %d)\n",X,NombreDeTermes,Pne->NombreDeVariablesTrav);   
		  for ( i = 0 ; i < NombreDeTermes ; i++ ) {
		    printf(" %e (%d) + ",Coefficient[i],IndiceDeLaVariable[i]);
		  }
		  printf(" < %e\n",SecondMembre);
		  fflush( stdout );			
			*/
			
		if ( Pne->pne_params->NORMALISER_LES_COUPES_SUR_LES_G_ET_I == OUI_PNE && PlusGrandCoeff >= Pne->pne_params->SEUIL_POUR_NORMALISER_LES_COUPES_SUR_LES_G_ET_I ) {
			PNE_NormaliserUnCoupe(Pne->Coefficient_CG, &SecondMembre, NombreDeTermes, PlusGrandCoeff);
		}
			
			PNE_EnrichirLeProblemeCourantAvecUneCoupe( Pne, 'I', NombreDeTermes, SecondMembre, X,
                                                 Coefficient, IndiceDeLaVariable );

			if ( Pne->pne_params->KNAPSACK_SUR_COUPE_DINTERSECTION == OUI_PNE ) {
				/* Il faut le faire apres le stockage de la coupe car la recherche des knapsack modifie
				 Pne->Coefficient_CG et Pne->IndiceDeLaVariable_CG */
				 /* On regarde si on peut faire une K sur la coupe */
				if ( X > Pne->pne_params->VIOLATION_MIN_POUR_K_SUR_COUPE && OnAEcrete != OUI_SPX ) {
					PNE_CalculerUneKnapsackSurGomoryOuIntersection(Pne, Coefficient, IndiceDeLaVariable,
						SecondMembre, NombreDeTermes, PlusGrandCoeff);
				}
			}
																								 
    }
  }
		
}

SPX_TerminerLeCalculDesCoupesDIntersection( (PROBLEME_SPX *) Pne->ProblemeSpxDuSolveur );

return;
}

