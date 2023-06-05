/*
** Copyright 2007-2018 RTE
** Author: Robert Gonzalez
**
** This file is part of Sirius_Solver.
** This program and the accompanying materials are made available under the
** terms of the Eclipse Public License 2.0 which is available at
** http://www.eclipse.org/legal/epl-2.0.
**
** This Source Code may also be made available under the following Secondary
** Licenses when the conditions for such availability set forth in the Eclipse
** Public License, v. 2.0 are satisfied: GNU General Public License, version 3
** or later, which is available at <http://www.gnu.org/licenses/>.
**
** SPDX-License-Identifier: EPL-2.0 OR GPL-3.0
*/
/***********************************************************************

   FONCTION: Ecriture du jeu de donnees au format MPS
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "pne_sys.h"
# include "pne_define.h"
# include "pne_fonctions.h"
# ifdef PNE_UTILISER_LES_OUTILS_DE_GESTION_MEMOIRE_PROPRIETAIRE	
  # include "pne_memoire.h"
# endif
#define SRS_BUFFER_SIZE 1024
/*----------------------------------------------------------------------------*/
void MettreAjourNom(int NomParDEfaut, char* Nom,  const char* NouveauNom, int Numero, char Type){
  memset(Nom, 0, SRS_BUFFER_SIZE);
  if (NomParDEfaut)
  {  
    snprintf(Nom, SRS_BUFFER_SIZE, "%c%07d",Type, Numero);
  }
  else
  {
    strncpy(Nom, NouveauNom, SRS_BUFFER_SIZE);
  }
}
void PNE_EcrireJeuDeDonneesMPS(PROBLEME_PNE * Pne, PROBLEME_A_RESOUDRE * Probleme) {
	PNE_EcrireJeuDeDonneesMPS_avecNom(Pne, Probleme, "Donnees_Probleme_Solveur.mps");
}

void PNE_EcrireJeuDeDonneesMPS_avecNom(PROBLEME_PNE * Pne, PROBLEME_A_RESOUDRE * Probleme, const char * const nomFichier)
{
FILE * Flot; 
int Cnt; int Var; int il; int ilk; int ilMax; char * Nombre;
int * Cder; int * Cdeb; int * NumeroDeContrainte; int * Csui; 

/*                                                        */
int NombreDeVariables; int * TypeDeVariable; int * TypeDeBorneDeLaVariable; 
double * Xmax; double * Xmin; double * CoutLineaire; int NombreDeContraintes;   
double objective_offset;
double * SecondMembre; char * Sens; int * IndicesDebutDeLigne;
int * NombreDeTermesDesLignes;	double * CoefficientsDeLaMatriceDesContraintes;
int * IndicesColonnes;
char** NomsDesVariables;
char** NomsDesContraintes;
char NomVariable[SRS_BUFFER_SIZE];
char NomContrainte[SRS_BUFFER_SIZE];
/*                                                        */

NombreDeVariables       = Probleme->NombreDeVariables; 
TypeDeVariable          = Probleme->TypeDeVariable; 
TypeDeBorneDeLaVariable = Probleme->TypeDeBorneDeLaVariable; 
Xmax                    = Probleme->Xmax;
Xmin                    = Probleme->Xmin; 
CoutLineaire            = Probleme->CoutLineaire; 
NombreDeContraintes                   = Probleme->NombreDeContraintes;   
SecondMembre                          = Probleme->SecondMembre; 
Sens                                  = Probleme->Sens; 
IndicesDebutDeLigne                   = Probleme->IndicesDebutDeLigne; 
NombreDeTermesDesLignes               = Probleme->NombreDeTermesDesLignes;	
CoefficientsDeLaMatriceDesContraintes = Probleme->CoefficientsDeLaMatriceDesContraintes; 
IndicesColonnes                       = Probleme->IndicesColonnes;      
objective_offset = Probleme->objective_offset;
NomsDesVariables = Probleme->NomsDesVariables;
NomsDesContraintes = Probleme->NomsDesContraintes;

/* Chainage de la transposee */
for ( ilMax = -1 , Cnt = 0 ; Cnt < NombreDeContraintes; Cnt++ ) {
  if ( ( IndicesDebutDeLigne[Cnt] + NombreDeTermesDesLignes[Cnt] - 1 ) > ilMax ) {
    ilMax = IndicesDebutDeLigne[Cnt] + NombreDeTermesDesLignes[Cnt] - 1;
  }
}
ilMax+= NombreDeContraintes; /* Marge */

Cder               = (int *) malloc( NombreDeVariables * sizeof( int ) ); 
Cdeb               = (int *) malloc( NombreDeVariables * sizeof( int ) ); 
NumeroDeContrainte = (int *) malloc( ilMax             * sizeof( int ) ); 
Csui               = (int *) malloc( ilMax             * sizeof( int ) ); 
Nombre             = (char *) malloc( 1024 ); 
if ( Cder == NULL || Cdeb == NULL || NumeroDeContrainte == NULL || Csui == NULL || Nombre == NULL ) {
  printf("Memoire insuffisante dans le sous programme PNE_EcrireJeuDeDonneesMPS\n"); 
  Pne->AnomalieDetectee = OUI_PNE;
  longjmp( Pne->Env , Pne->AnomalieDetectee ); 
}

for ( Var = 0 ; Var < NombreDeVariables ; Var++ ) Cdeb[Var] = -1;
for ( Cnt = 0 ; Cnt < NombreDeContraintes ; Cnt++ ) {
  il    = IndicesDebutDeLigne[Cnt];
  ilMax = il + NombreDeTermesDesLignes[Cnt];
  while ( il < ilMax ) {
    Var = IndicesColonnes[il];
    if ( Cdeb[Var] < 0 ) {
      Cdeb              [Var] = il;
      NumeroDeContrainte[il]  = Cnt;
      Csui              [il]  = -1;
      Cder              [Var] = il;
    }
    else {
      ilk                     = Cder[Var];
      Csui              [ilk] = il;
      NumeroDeContrainte[il]  = Cnt;
      Csui              [il]  = -1;
      Cder              [Var] = il;
    }
    il++;
  }
}
free( Cder );
/* Fin chainage de la transposee */

Flot = fopen( nomFichier, "w" );
if( Flot == NULL ) {
  printf("Erreur ouverture du fichier pour l'ecriture du jeu de donnees %s\n", nomFichier);
  exit(0);
}

/* Ecrire du titre */
fprintf(Flot,"* Number of variables:   %d\n",NombreDeVariables);
fprintf(Flot,"* Number of constraints: %d\n",NombreDeContraintes);

/*
 Les champs du format MPS
Champ1 :  2- 3
Champ2 :  5-12
Champ3 : 15-22
Champ4 : 25-36
Champ5 : 40-47
Champ6 : 50-61  
*/
 
/* NAME */    
fprintf(Flot,"NAME          Pb Solve\n");

/* ROWS */
fprintf(Flot,"ROWS\n");
/*
In this section all the row labels are defined, as well as the row type. The row 
type is entered in field 1 (in column 2 or 3) and the row label is entered in 
field 2 (columns 5-12). Row type:
E : egalité
L : inferieur ou egal
G : superieur ou egal
N : objectif			
N : free ??
*/
/* Objectif */
fprintf(Flot," N  OBJECTIF\n");
/* Ecriture de toutes les contraintes */
for ( Cnt = 0 ; Cnt < NombreDeContraintes ; Cnt++ ) {
  int NomDeContrainteParDefaut = NomsDesContraintes == NULL || NomsDesContraintes[Cnt] == NULL;
  MettreAjourNom(NomDeContrainteParDefaut, NomContrainte,NomsDesContraintes[Cnt], Cnt, 'R' );
  if ( Sens[Cnt] == '=' ) {
    fprintf(Flot, " E  %s\n", NomContrainte);
  }
  else if (  Sens[Cnt] == '<' ) {
    fprintf(Flot, " L  %s\n", NomContrainte);
  }
  else if (  Sens[Cnt] == '>' ) {
    fprintf(Flot, " G  %s\n", NomContrainte);
  }
  else {
    fprintf(Flot,"PNE_EcrireJeuDeDonneesMPS : le sens de la contrainte %c ne fait pas partie des sens reconnus\n",
            Sens[Cnt]);
    exit(0);
  } 
  
}

/* COLUMNS */
fprintf(Flot,"COLUMNS\n");
for ( Var = 0 ; Var < NombreDeVariables ; Var++ ) {
  int NomDeVariableParDefaut = NomsDesVariables == NULL || NomsDesVariables[Var] == NULL;
  
  MettreAjourNom(NomDeVariableParDefaut, NomVariable, NomsDesVariables[Var], Var, 'C');
  if (CoutLineaire[Var] != 0.0)
  {
    sprintf(Nombre, "%-.10lf", CoutLineaire[Var]);
    /*Nombre[12] = '\0';*/ /* <- On prefere ne pas ajouter de troncature */
    fprintf(Flot, "    %s  OBJECTIF  %s\n", NomVariable, Nombre);
  }
  il = Cdeb[Var];
  while (il >= 0)
  {
    Cnt = NumeroDeContrainte[il];
    int NomDeContrainteParDefaut = NomsDesContraintes == NULL || NomsDesContraintes[Cnt] == NULL;
    MettreAjourNom(NomDeContrainteParDefaut, NomContrainte, NomsDesContraintes[Cnt], Cnt, 'R');
    
    sprintf(Nombre, "%-.10lf", CoefficientsDeLaMatriceDesContraintes[il]);
    /*Nombre[12] = '\0';*/ /* <- On prefere ne pas ajouter de troncature */
    fprintf(Flot, "    %s  %s  %s\n", NomVariable, NomContrainte, Nombre);

    il = Csui[il];
  }
}

/* RHS */
fprintf(Flot,"RHS\n");
// Objective offset
// see https://www.ibm.com/docs/en/icos/20.1.0?topic=standard-records-in-mps-format
// NOTE: By convention, we write here the negative objective value
if (objective_offset != 0.0) {
   fprintf(Flot,"    RHSVAL    OBJECTIF  %f\n",  -objective_offset);
}
for (Cnt = 0; Cnt < NombreDeContraintes; Cnt++)
{
   if (SecondMembre[Cnt] != 0.0)
   {

    int NomDeContrainteParDefaut = NomsDesContraintes == NULL || NomsDesContraintes[Cnt] == NULL;
    MettreAjourNom(NomDeContrainteParDefaut, NomContrainte, NomsDesContraintes[Cnt], Cnt, 'R');
    sprintf(Nombre, "%-.9lf", SecondMembre[Cnt]);
    /*Nombre[12] = '\0';*/ /* <- On prefere ne pas ajouter de troncature */
    fprintf(Flot, "    RHSVAL    %s  %s\n", NomContrainte, Nombre);
   }
}

/* BOUNDS */
fprintf(Flot,"BOUNDS\n");
/*
 Field 1 (columns 2-3) specifies the type of bound:
 LO lower bound
 UP upper bound
 LI lower bound integer variable
 UI upper bound integer variable
 BV binary variable
 FX fixed variable
 FR free 
 MI lower bound - infini
 PL upper bound + infini 
*/
for ( Var = 0 ; Var < NombreDeVariables ; Var++ ) {
  int NomDeVariableParDefaut = NomsDesVariables == NULL || NomsDesVariables[Var] == NULL;
  MettreAjourNom(NomDeVariableParDefaut, NomVariable, NomsDesVariables[Var], Var, 'C');
  if ( TypeDeBorneDeLaVariable[Var] == VARIABLE_FIXE ) {
    sprintf(Nombre,"%-.9lf",Xmin[Var]);
    /*Nombre[12] = '\0';*/ /* <- On prefere ne pas ajouter de troncature */

    fprintf(Flot, " FX BNDVALUE  %s  %s\n", NomVariable, Nombre);

    continue;
  }
  if ( TypeDeVariable[Var] == ENTIER ) {
    fprintf(Flot, " BV BNDVALUE  %s\n", NomVariable);
    continue;
   }
  /* Variable reelle */
  /* Par defaut la variable est PL i.e;. comprise entre 0 et + l'infini */
  if ( TypeDeBorneDeLaVariable[Var] == VARIABLE_BORNEE_DES_DEUX_COTES ) {
    if (Xmin[Var] != 0.0)
    {
      sprintf(Nombre, "%-.9lf", Xmin[Var]);
      /*Nombre[12] = '\0';*/ /* <- On prefere ne pas ajouter de troncature */
      fprintf(Flot," LO BNDVALUE  %s  %s\n",NomVariable,Nombre);
    }
    sprintf(Nombre, "%-.9lf", Xmax[Var]);
    /*Nombre[12] = '\0';*/ /* <- On prefere ne pas ajouter de troncature */
    fprintf(Flot," UP BNDVALUE  %s  %s\n",NomVariable,Nombre);
  }
  if (TypeDeBorneDeLaVariable[Var] == VARIABLE_BORNEE_INFERIEUREMENT)
  {
    if (Xmin[Var] != 0.0)
    {
      sprintf(Nombre, "%-.9lf", Xmin[Var]);
      /*Nombre[12] = '\0';*/ /* <- On prefere ne pas ajouter de troncature */
      fprintf(Flot," LO BNDVALUE  %s  %s\n",NomVariable,Nombre);
    }
  }
  if ( TypeDeBorneDeLaVariable[Var] == VARIABLE_BORNEE_SUPERIEUREMENT ) {
    fprintf(Flot," MI BNDVALUE  %s\n",NomVariable);
    if ( Xmax[Var] != 0.0 ) {
      sprintf(Nombre,"%-.9lf",Xmax[Var]);
      /*Nombre[12] = '\0';*/ /* <- On prefere ne pas ajouter de troncature */
      fprintf(Flot," UP BNDVALUE  %s  %s\n",NomVariable,Nombre);
    }
  }
  if ( TypeDeBorneDeLaVariable[Var] == VARIABLE_NON_BORNEE ) {
    fprintf(Flot," FR BNDVALUE  %s\n",NomVariable);
  }
}

/* ENDDATA */
fprintf(Flot,"ENDATA\n");

free ( Cdeb );
free ( NumeroDeContrainte );
free ( Csui );
free ( Nombre );

fclose( Flot );
  
return;	  

}
