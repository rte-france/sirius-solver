// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Mise a jour du seuil utilise pour les coupes.
                
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

void PNE_MiseAJourSeuilCoupes( PROBLEME_PNE * Pne, char TypeDeCoupe, double * Seuil )
{
BB * Bb; char ProblemeUniquementEnVariablesEntieres; 

Bb = (BB *) Pne->ProblemeBbDuSolveur;
ProblemeUniquementEnVariablesEntieres = NON_PNE;
if ( Pne->NombreDeVariablesEntieresTrav == Pne->NombreDeVariablesNonFixes ) ProblemeUniquementEnVariablesEntieres = OUI_PNE;

if ( TypeDeCoupe == COUPE_KNAPSACK ) {
  if ( ProblemeUniquementEnVariablesEntieres == OUI_PNE ) *Seuil = 0.1 * Pne->pne_params->SEUIL_VIOLATION_KNAPSACK;
  else {
    /* Si pb mixte */
	  if ( Bb->AverageK == NON_INITIALISE ) *Seuil = Pne->pne_params->SEUIL_VIOLATION_KNAPSACK;
	  else {
      if ( Bb->AverageK > 0.25 * Bb->AverageG && Bb->NoeudEnExamen != Bb->NoeudRacine ) *Seuil = Pne->SeuilDeViolationK;
      else *Seuil = Pne->pne_params->SEUIL_VIOLATION_KNAPSACK;
	  }
  }
}
else if ( TypeDeCoupe == COUPE_MIR_MARCHAND_WOLSEY ) {
	if ( Bb->AverageK == NON_INITIALISE ) *Seuil = Pne->pne_params->SEUIL_VIOLATION_MIR_MARCHAND_WOLSEY;
  else {
    if ( Bb->AverageK > 0.25 * Bb->AverageG ) *Seuil = Pne->SeuilDeViolationMIR_MARCHAND_WOLSEY;
    else *Seuil = Pne->pne_params->SEUIL_VIOLATION_MIR_MARCHAND_WOLSEY;
  }
}
else if ( TypeDeCoupe == COUPE_CLIQUE ) {
  if ( ProblemeUniquementEnVariablesEntieres == OUI_PNE ) *Seuil = 0.1 * Pne->pne_params->SEUIL_VIOLATION_CLIQUES;
  else {
    /* Si pb mixte */
	  if ( Bb->AverageK == NON_INITIALISE ) *Seuil = Pne->pne_params->SEUIL_VIOLATION_CLIQUES;
    else {		
      if ( Bb->AverageK > 0.25 * Bb->AverageG && Bb->NoeudEnExamen != Bb->NoeudRacine ) *Seuil = Pne->SeuilDeViolationCliques;
      else *Seuil = Pne->pne_params->SEUIL_VIOLATION_CLIQUES;
	  }
  }
}
else if ( TypeDeCoupe == COUPE_IMPLICATION ) {
  if ( ProblemeUniquementEnVariablesEntieres == OUI_PNE ) *Seuil = 0.1 * Pne->pne_params->SEUIL_VIOLATION_IMPLICATIONS;
  else {
    /* Si pb mixte */
	  if ( Bb->AverageK == NON_INITIALISE ) *Seuil = Pne->pne_params->SEUIL_VIOLATION_IMPLICATIONS;
    else {		
      if ( Bb->AverageK > 0.25 * Bb->AverageG && Bb->NoeudEnExamen != Bb->NoeudRacine ) *Seuil = Pne->SeuilDeViolationImplications;
      else *Seuil = Pne->pne_params->SEUIL_VIOLATION_IMPLICATIONS;
	  }
  }
}
else if ( TypeDeCoupe == COUPE_DE_BORNE_VARIABLE ) {
  if ( ProblemeUniquementEnVariablesEntieres == OUI_PNE ) *Seuil = 0.1 * Pne->pne_params->SEUIL_VIOLATION_BORNES_VARIABLES;
  else {
    /* Si pb mixte */
	  if ( Bb->AverageK == NON_INITIALISE ) *Seuil = Pne->pne_params->SEUIL_VIOLATION_BORNES_VARIABLES;
    else {		
      if ( Bb->AverageK > 0.25 * Bb->AverageG && Bb->NoeudEnExamen != Bb->NoeudRacine ) *Seuil = Pne->SeuilDeViolationBornesVariables;
      else *Seuil = Pne->pne_params->SEUIL_VIOLATION_BORNES_VARIABLES;
	  }
  }
}
else {
  printf("PNE_MiseAJourSeuilCoupes bug: TypeDeCoupe %d inconnu\n",TypeDeCoupe);
	exit(0);
}

return;
}

/*----------------------------------------------------------------------------*/

void PNE_MiseAJourDesSeuilDeSelectionDesCoupes( PROBLEME_PNE * Pne )
{
BB * Bb; char ProblemeUniquementEnVariablesEntieres; double CoeffMin; double CoeffMax;

/* Seuil pour les Knapsack afin d'eviter d'en avoir trop */ 
Bb = Pne->ProblemeBbDuSolveur;
ProblemeUniquementEnVariablesEntieres = NON_PNE;
if ( Pne->NombreDeVariablesEntieresTrav == Pne->NombreDeVariablesNonFixes ) ProblemeUniquementEnVariablesEntieres = OUI_PNE;

if ( Bb->NombreDeProblemesResolusDepuisLaRAZDesCoupes > 0 ) {
	Bb->AverageG = (int) ceil(Bb->NombreTotalDeGDuPoolUtilisees/Bb->NombreDeProblemesResolusDepuisLaRAZDesCoupes);
  Bb->AverageK = (int) ceil(Bb->NombreTotalDeKDuPoolUtilisees/Bb->NombreDeProblemesResolusDepuisLaRAZDesCoupes);
}

if ( Bb->AverageK != NON_INITIALISE ) {
  if ( Bb->AverageK > 0.25 * Bb->AverageG ) {
    if ( Pne->NombreDeK > 0 ) Pne->SeuilDeViolationK = Pne->SommeViolationsK / Pne->NombreDeK;
    if ( Pne->NombreDeMIR_MARCHAND_WOLSEY > 0 ) Pne->SeuilDeViolationMIR_MARCHAND_WOLSEY = Pne->SommeViolationsMIR_MARCHAND_WOLSEY / Pne->NombreDeMIR_MARCHAND_WOLSEY;
    if ( Pne->NombreDeCliques > 0 ) Pne->SeuilDeViolationCliques = Pne->SommeViolationsCliques / Pne->NombreDeCliques;
    if ( Pne->NombreDImplications > 0 ) Pne->SeuilDeViolationImplications = Pne->SommeViolationsImplications / Pne->NombreDImplications;
    if ( Pne->NombreDeBornesVariables > 0 ) Pne->SeuilDeViolationBornesVariables = Pne->SommeViolationsBornesVariables / Pne->NombreDeBornesVariables;
	}
}

CoeffMin = 1.e-2;
CoeffMax = 1.e+2;
if ( Pne->SeuilDeViolationK > CoeffMax * Pne->pne_params->SEUIL_VIOLATION_KNAPSACK ) Pne->SeuilDeViolationK = CoeffMax * Pne->pne_params->SEUIL_VIOLATION_KNAPSACK;
else if( Pne->SeuilDeViolationK < CoeffMin * Pne->pne_params->SEUIL_VIOLATION_KNAPSACK ) Pne->SeuilDeViolationK = CoeffMin * Pne->pne_params->SEUIL_VIOLATION_KNAPSACK;

if ( Pne->SeuilDeViolationMIR_MARCHAND_WOLSEY > CoeffMax * Pne->pne_params->SEUIL_VIOLATION_MIR_MARCHAND_WOLSEY ) Pne->SeuilDeViolationMIR_MARCHAND_WOLSEY = CoeffMax * Pne->pne_params->SEUIL_VIOLATION_MIR_MARCHAND_WOLSEY;
else if ( Pne->SeuilDeViolationMIR_MARCHAND_WOLSEY < CoeffMin * Pne->pne_params->SEUIL_VIOLATION_MIR_MARCHAND_WOLSEY  ) Pne->SeuilDeViolationMIR_MARCHAND_WOLSEY = CoeffMin * Pne->pne_params->SEUIL_VIOLATION_MIR_MARCHAND_WOLSEY;

if ( Pne->SeuilDeViolationCliques > CoeffMax * Pne->pne_params->SEUIL_VIOLATION_CLIQUES ) Pne->SeuilDeViolationCliques = CoeffMax * Pne->pne_params->SEUIL_VIOLATION_CLIQUES;
else if ( Pne->SeuilDeViolationCliques < CoeffMin * Pne->pne_params->SEUIL_VIOLATION_CLIQUES ) Pne->SeuilDeViolationCliques = CoeffMin * Pne->pne_params->SEUIL_VIOLATION_CLIQUES;

if ( Pne->SeuilDeViolationImplications > CoeffMax * Pne->pne_params->SEUIL_VIOLATION_IMPLICATIONS ) Pne->SeuilDeViolationImplications = CoeffMax * Pne->pne_params->SEUIL_VIOLATION_IMPLICATIONS;
else if ( Pne->SeuilDeViolationImplications < CoeffMin * Pne->pne_params->SEUIL_VIOLATION_IMPLICATIONS ) Pne->SeuilDeViolationImplications = CoeffMin * Pne->pne_params->SEUIL_VIOLATION_IMPLICATIONS;

if ( Pne->SeuilDeViolationBornesVariables > CoeffMax * Pne->pne_params->SEUIL_VIOLATION_BORNES_VARIABLES ) Pne->SeuilDeViolationBornesVariables = CoeffMax * Pne->pne_params->SEUIL_VIOLATION_BORNES_VARIABLES;
else if ( Pne->SeuilDeViolationBornesVariables < CoeffMin * Pne->pne_params->SEUIL_VIOLATION_BORNES_VARIABLES ) Pne->SeuilDeViolationBornesVariables = CoeffMin * Pne->pne_params->SEUIL_VIOLATION_BORNES_VARIABLES;

/*
printf("SeuilDeViolationK %e  SeuilDeViolationMIR_MARCHAND_WOLSEY %e SeuilDeViolationCliques %e SeuilDeViolationImplications %e SeuilDeViolationBornesVariables %e\n",
        Pne->SeuilDeViolationK,Pne->SeuilDeViolationMIR_MARCHAND_WOLSEY,Pne->SeuilDeViolationCliques,
				Pne->SeuilDeViolationImplications,Pne->SeuilDeViolationBornesVariables);
*/

return;
}
