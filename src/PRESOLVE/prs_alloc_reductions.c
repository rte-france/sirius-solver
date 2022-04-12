// Copyright (C) 2007-2018, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/***********************************************************************

   FONCTION: Alloc des structures pour les operations de reduction du
	           presolve.

						 RQ: transfere dans pne
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "prs_sys.h"

# include "prs_fonctions.h"
# include "prs_define.h"

# include "pne_fonctions.h"
# include "pne_define.h"
   
# define TRACES 0																												

/*----------------------------------------------------------------------------*/
void PRS_AllocTablesDeSubstitution( void * ProblemePne )  
{
int NombreDeVariables; int ilMax; PROBLEME_PNE * Pne;

Pne = (PROBLEME_PNE *) ProblemePne;

NombreDeVariables = Pne->NombreDeVariablesTrav;
ilMax = Pne->TailleAlloueePourLaMatriceDesContraintes;

Pne->IndexLibreVecteurDeSubstitution = 0;
Pne->NbVariablesSubstituees = 0; 

Pne->NumeroDesVariablesSubstituees = (int *) malloc( NombreDeVariables * sizeof( int ) );
if ( Pne->NumeroDesVariablesSubstituees == NULL )  return;

Pne->ValeurDeLaConstanteDeSubstitution = (double *) malloc( NombreDeVariables * sizeof( double ) );
if ( Pne->ValeurDeLaConstanteDeSubstitution == NULL ) {
  free( Pne->NumeroDesVariablesSubstituees );
	return;
}

Pne->IndiceDebutVecteurDeSubstitution = (int *) malloc( NombreDeVariables * sizeof( int ) );
if ( Pne->IndiceDebutVecteurDeSubstitution == NULL ) {
  free( Pne->NumeroDesVariablesSubstituees ); free( Pne->ValeurDeLaConstanteDeSubstitution );
	return;
}

Pne->NbTermesVecteurDeSubstitution = (int *) malloc( NombreDeVariables * sizeof( int ) );
if ( Pne->NbTermesVecteurDeSubstitution == NULL ) {
  free( Pne->NumeroDesVariablesSubstituees ); free( Pne->ValeurDeLaConstanteDeSubstitution );
  free( Pne->IndiceDebutVecteurDeSubstitution );
	return;
}

Pne->CoeffDeSubstitution = (double *) malloc( ilMax * sizeof( double ) );
if ( Pne->CoeffDeSubstitution == NULL ) {
  free( Pne->NumeroDesVariablesSubstituees ); free( Pne->ValeurDeLaConstanteDeSubstitution );
  free( Pne->IndiceDebutVecteurDeSubstitution ); free( Pne->NbTermesVecteurDeSubstitution );
	return;
}

Pne->NumeroDeVariableDeSubstitution = (int *) malloc( ilMax * sizeof( int ) );
if ( Pne->NumeroDeVariableDeSubstitution == NULL ) {
  free( Pne->NumeroDesVariablesSubstituees ); free( Pne->ValeurDeLaConstanteDeSubstitution );
  free( Pne->IndiceDebutVecteurDeSubstitution ); free( Pne->NbTermesVecteurDeSubstitution );
  free( Pne->CoeffDeSubstitution );
	return;
}

Pne->CoutDesVariablesSubstituees = (double *) malloc( NombreDeVariables * sizeof( double ) );
if ( Pne->CoutDesVariablesSubstituees == NULL )  {
  free( Pne->NumeroDesVariablesSubstituees ); free( Pne->ValeurDeLaConstanteDeSubstitution );
  free( Pne->IndiceDebutVecteurDeSubstitution ); free( Pne->NbTermesVecteurDeSubstitution );
  free( Pne->CoeffDeSubstitution ); free( Pne->NumeroDeVariableDeSubstitution );
  return;
}

Pne->ContrainteDeLaSubstitution = (int *) malloc( NombreDeVariables * sizeof( int ) );
if ( Pne->ContrainteDeLaSubstitution == NULL )  {
  free( Pne->NumeroDesVariablesSubstituees ); free( Pne->ValeurDeLaConstanteDeSubstitution );
  free( Pne->IndiceDebutVecteurDeSubstitution ); free( Pne->NbTermesVecteurDeSubstitution );
  free( Pne->CoeffDeSubstitution ); free( Pne->NumeroDeVariableDeSubstitution );
  free( Pne->CoutDesVariablesSubstituees );
  return;
}

return;
}

