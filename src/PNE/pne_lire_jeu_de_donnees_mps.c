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

   FONCTION: Lecture du jeu de donnees au format MPS
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "pne_sys.h"
# include "pne_define.h"
# include "pne_fonctions.h"

# include "mps_define.h"
# include "mps_global.h"

int aHashCode = 31;
int kHashCode = 0;
  
# define OFFSET_BASIS    0 /*2166136261*/
# define FNV_PRIME       16777619

/*----------------------------------------------------------------------------*/

void PNE_LireJeuDeDonneesMPS() {
	PNE_LireJeuDeDonneesMPS_AvecNom(&Mps, "A_JEU_DE_DONNEES");
}

void PNE_LireJeuDeDonneesMPS_AvecNom(PROBLEME_MPS * problemMps, const char * nomFichier)
{
FILE * Flot; 
int LgMax     ; int Count; int PositionDansLeFichier; int i         ; 
int CntCourant; int Cnt  ; int ilk                  ; int NbChamps  ; 
int ilMps     ; int il   ; int NbInit               ; int CountVar  ;  
int CountTrm  ; int VariablesBinairesEnCours         ; 
int VarCourant; int BaseVar                          ; int BaseCnt   ;
int j; double SensOpt;

double PlusLinfiniBis; double CoefficientMps; double CoefficientMpsB; 

double * A; int * Nuvar; int * Mdeb; int * NbTerm; 

char * LigneLue;  
char   TypeDeContrainte[2]     ; char * LabelDeLaContrainteLue     ; 
char * LabelDeLaVariableLue    ; char * LabelDeLaVariablePrecedente;
char * LabelDeLaContrainteLueB ; char * LabelDuSecondMembreLu      ;
char * ValeurLue               ; char * ValeurLueB                 ;
char * ValeurLueC              ; char   TypeDeBorne[3]             ; 
char * LabelDeLaBorneLue       ;

/* SensOpt = 1 si minimisation et SensOpt = -1. si maximisation */
SensOpt = 1.;

problemMps->CoeffHaschCodeContraintes = 6;
problemMps->SeuilHaschCodeContraintes = 8;

problemMps->CoeffHaschCodeVariables = 3;
problemMps->SeuilHaschCodeVariables = 8;

Flot = fopen(nomFichier, "r" );
if( Flot == NULL ) {
  printf("Erreur ouverture du fichier contenant le jeu de donnees \n");
  exit(0);
}

LgMax = 1024;
LigneLue                    = (char *) malloc( LgMax );
LabelDeLaContrainteLue      = (char *) malloc( LgMax );
LabelDeLaVariableLue        = (char *) malloc( LgMax );
LabelDeLaVariablePrecedente = (char *) malloc( LgMax );
LabelDeLaContrainteLueB     = (char *) malloc( LgMax );
LabelDuSecondMembreLu       = (char *) malloc( LgMax );
LabelDeLaBorneLue           = (char *) malloc( LgMax );
ValeurLue                   = (char *) malloc( LgMax );
ValeurLueB                  = (char *) malloc( LgMax );
ValeurLueC                  = (char *) malloc( LgMax );
if ( LigneLue                == NULL || LabelDeLaContrainteLue      == NULL || 
     LabelDeLaVariableLue    == NULL || LabelDeLaVariablePrecedente == NULL ||
     LabelDeLaContrainteLueB == NULL || LabelDuSecondMembreLu       == NULL ||
     LabelDeLaBorneLue       == NULL || ValeurLue                   == NULL ||
     ValeurLueB              == NULL || ValeurLueC                  == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_LireJeuDeDonneesMPS \n");
  exit(0);
}

problemMps->NbVar = -1;
problemMps->NbCnt = -1;

PlusLinfiniBis = 1.e+70 /*1.e+8*/; 

/*
 Les champs du format MPS
Champ1 :  2- 3
Champ2 :  5-12
Champ3 : 15-22
Champ4 : 25-36
Champ5 : 40-47
Champ6 : 50-61  
*/
 
/*               Lecture des lignes             */
/* The section is preceded by a card with ROWS in columns 1-4 */
while ( 1 ) {
  fgets( LigneLue , LgMax , Flot );

  if( strstr( LigneLue , "OBJSENSE" ) == LigneLue ) {
	  /* Dans ce cas on verifie s'il s'agit d'un maximisation ou d'une minimisation */
		/* On verifie s'il le maximize ou le minimize est sur la ligne sinon on le cherche dans les lignes suivantes */
    if( strstr( LigneLue , "MAXIMIZE" ) != NULL ) {
		  SensOpt = -1.;
		}
		else if( strstr( LigneLue , "MINIMIZE" ) != NULL ) {
		  SensOpt = 1.;
		}
		else {		
      fgets( LigneLue , LgMax , Flot );		
      if( strstr( LigneLue , "MAXIMIZE" ) != NULL ) {
		    SensOpt = -1.;
		  }
		  else if( strstr( LigneLue , "MINIMIZE" ) != NULL ) {
		    SensOpt = 1.;
		  }
			else {
			  printf("Sens de l'opimisation (MINIMIZE ou MAXIMIZE) pas trouve dans les donnees\n");
				exit(0);
			}
		}
	}
	
  if( strstr( LigneLue , "ROWS" ) == LigneLue ) break;
}

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
/*printf("Debut de la lecture des cartes ROWS \n ");*/
PositionDansLeFichier = ftell( Flot ); /* Stockage du debut des informations sur ROWS */

Count = 0;
while ( 1 ) { /* Pour savoir combien de contraintes il faut allouer */
  fgets( LigneLue , LgMax , Flot );
  if( strstr( LigneLue , "COLUMNS" ) == LigneLue ) break;
  Count++;
}
/* Allocations */
problemMps->Mdeb                = (int *) malloc( Count * sizeof( int ) );
problemMps->Mder                = (int *) malloc( Count * sizeof( int ) );
problemMps->NbTerm              = (int *) malloc( Count * sizeof( int ) );
problemMps->Rhs                   = (double *) malloc( Count * sizeof( double ) );
problemMps->SensDeLaContrainte  = (char *)   malloc( Count * sizeof( char   ) );
problemMps->BRange              = (double *) malloc( Count * sizeof( double ) );
problemMps->VariablesDualesDesContraintes = (double *) malloc( Count * sizeof( double ) );
problemMps->LabelDeLaContrainte = (char **)  malloc( Count * sizeof( void * ) );
problemMps->LabelDuSecondMembre = (char **)  malloc( Count * sizeof( void * ) );
if ( problemMps->Mdeb                == NULL || problemMps->Mder   == NULL || 
     problemMps->NbTerm              == NULL || problemMps->Rhs      == NULL ||
     problemMps->SensDeLaContrainte  == NULL || problemMps->BRange == NULL ||
		 problemMps->VariablesDualesDesContraintes == NULL ||
		 problemMps->LabelDeLaContrainte == NULL           ||
		 problemMps->LabelDuSecondMembre == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_LireJeuDeDonneesMPS \n");
  exit(0);
}

problemMps->NbCntRange = 0;
for ( i = 0; i < Count ; i++ ) {
  problemMps->Rhs[i]      = 0.0;
	problemMps->BRange[i] = 0.0;
}

problemMps->LabelDeLObjectif = NULL;

fseek( Flot , PositionDansLeFichier , SEEK_SET ); /* Repositionnement dans le fichier */
while ( 1 ) {
  fgets( LigneLue , LgMax , Flot );
	
  if( strstr( LigneLue , "COLUMNS" ) == LigneLue ) break;

  NbChamps = sscanf( LigneLue , "%s  %s" , TypeDeContrainte, LabelDeLaContrainteLue);
  if ( NbChamps <= 0 ) continue;  

  if ( TypeDeContrainte[0] == 'N' )  {
	  if ( problemMps->LabelDeLObjectif == NULL ) {
	    /* Objectif */
      problemMps->LabelDeLObjectif = (char *) malloc( strlen( LabelDeLaContrainteLue ) + 1 );
      strcpy( problemMps->LabelDeLObjectif , LabelDeLaContrainteLue );
      continue;
		}
  }
	
  problemMps->NbCnt++;
  problemMps->LabelDeLaContrainte[problemMps->NbCnt] = (char *) malloc( strlen( LabelDeLaContrainteLue ) + 1 ); 
  strcpy( problemMps->LabelDeLaContrainte[problemMps->NbCnt] , LabelDeLaContrainteLue );
  /*printf("Contrainte lue : %s\n",problemMps->LabelDeLaContrainte[problemMps->NbCnt]);*/
  problemMps->Rhs[problemMps->NbCnt] = 0.;
  if ( TypeDeContrainte[0] == 'E' )  { /* Egalite */
    problemMps->SensDeLaContrainte[problemMps->NbCnt] = '=';
    continue;
  }  
  if ( TypeDeContrainte[0] == 'L' )  { /* Inferieur ou egal */
    problemMps->SensDeLaContrainte[problemMps->NbCnt] = '<';
    continue;
  }
  if ( TypeDeContrainte[0] == 'G' )  { /* Superieur ou egal */
    problemMps->SensDeLaContrainte[problemMps->NbCnt] = '>';
    continue;
  }
  if ( TypeDeContrainte[0] == 'N' )  { /* Free */
    problemMps->SensDeLaContrainte[problemMps->NbCnt] = 'N';
    continue;
  }	
  printf(" Type de contrainte non reconnu: %s \n",TypeDeContrainte);
  exit(0);
}
problemMps->NbCnt++; /* Important */

BaseCnt = 0;
PNE_CreerHashCodeContrainteMPS( /*&BaseCnt*/ );	    

/*     Lecture des colonnes   */
/*
printf("Debut de la lecture des cartes COLUMNS \n"); fflush(stdout);  
*/
/* The section is preceded by a card with COLUMNS in columns 1-7 */
PositionDansLeFichier = ftell( Flot ); /* Stockage du debut des informations sur ROWS */
 
memset( (char *) LabelDeLaVariablePrecedente , ' ' ,  LgMax * sizeof( char ) );
CountVar = 0;
CountTrm = 0;
CoefficientMpsB = 0.0; /* Pour ne pas etre embete par les warning de compilation */
while ( 1 ) { /* Pour savoir combien de variables il faut allouer */

  memset( (char *) LabelDeLaContrainteLue      , ' ' ,  LgMax * sizeof( char ) );
  memset( (char *) LabelDeLaContrainteLue      , ' ' ,  LgMax * sizeof( char ) );
  memset( (char *) LabelDeLaContrainteLueB     , ' ' ,  LgMax * sizeof( char ) );
  memset( (char *) LabelDeLaVariableLue        , ' ' ,  LgMax * sizeof( char ) );

  fgets( LigneLue , LgMax , Flot );
	
  if( strstr( LigneLue , "RHS" ) == LigneLue ) break;
  if( strstr( LigneLue , "ENDATA" ) == LigneLue ) goto FinLecture;
    
    NbChamps = sscanf( LigneLue , "%s %s %s %s %s" ,
                       LabelDeLaVariableLue    , LabelDeLaContrainteLue  , ValeurLue ,
                       LabelDeLaContrainteLueB , ValeurLueB );
    if ( NbChamps <= 0 ) continue; 
    
    CoefficientMps  = atof( ValeurLue );
    if ( NbChamps > 3 ) CoefficientMpsB = atof( ValeurLueB );
     
    if ( strcmp( LabelDeLaVariableLue , LabelDeLaVariablePrecedente ) != 0 ) { /* Nouvelle variable */
      CountVar++;
    }
    strcpy( LabelDeLaVariablePrecedente , LabelDeLaVariableLue );
    CountTrm++;
    if ( NbChamps > 3 ) CountTrm++;

}

/* Allocations des tableaux */
problemMps->LabelDeLaVariable       = (char **)  malloc( CountVar * sizeof( void * ) );
problemMps->TypeDeVariable          = (int *)   malloc( CountVar * sizeof( int   ) );
problemMps->TypeDeBorneDeLaVariable = (int *)   malloc( CountVar * sizeof( int   ) );
problemMps->U                       = (double *) malloc( CountVar * sizeof( double ) );
problemMps->CoefsObjectif                       = (double *) malloc( CountVar * sizeof( double ) );
problemMps->Umin                    = (double *) malloc( CountVar * sizeof( double ) );
problemMps->Umax                    = (double *) malloc( CountVar * sizeof( double ) );

problemMps->A     = (double *) malloc( CountTrm * sizeof( double ) );
problemMps->Nuvar = (int *)   malloc( CountTrm * sizeof( int   ) );
problemMps->Msui  = (int *  ) malloc( CountTrm * sizeof( int   ) );

if ( problemMps->LabelDeLaVariable    == NULL || problemMps->TypeDeVariable == NULL || problemMps->TypeDeBorneDeLaVariable == NULL || 
     problemMps->U                    == NULL || problemMps->CoefsObjectif              == NULL || problemMps->Umin                    == NULL ||
     problemMps->Umax                 == NULL || problemMps->A              == NULL || problemMps->Nuvar                   == NULL ||
     problemMps->Msui                 == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_LireJeuDeDonneesMPS \n");
  exit(0);
}

for ( i = 0 ; i < CountVar ; i++ ) problemMps->CoefsObjectif[i] = 0.0;

/* Phase effective de lecture */
memset( (char *) problemMps->NbTerm , 0 , problemMps->NbCnt * sizeof( int ) );
for ( i = 0 ; i < problemMps->NbCnt ; i++ ) { 
  problemMps->Mdeb[i] = -1; 
  problemMps->Mder[i] = -1; 
}

CntCourant = 0;
il         = -1;
memset( (char *) LabelDeLaVariablePrecedente , ' ' ,  LgMax * sizeof( char ) );

VariablesBinairesEnCours = NON_MPS;

fseek( Flot , PositionDansLeFichier , SEEK_SET ); /* Repositionnement dans le fichier */
while ( 1 ) {

  fgets( LigneLue , LgMax , Flot );	
  if( strstr( LigneLue , "RHS" ) == LigneLue ) break;

  memset( (char *) LabelDeLaContrainteLue      , ' ' ,  LgMax * sizeof( char ) );
  memset( (char *) LabelDeLaContrainteLue      , ' ' ,  LgMax * sizeof( char ) );
  memset( (char *) LabelDeLaContrainteLueB     , ' ' ,  LgMax * sizeof( char ) );
  memset( (char *) LabelDeLaVariableLue        , ' ' ,  LgMax * sizeof( char ) );

  if ( strstr( LigneLue , "MARKER" ) != 0 &&  strstr( LigneLue , "INTORG" ) != 0 ) {
    /* printf("Lecture de variables entieres \n"); */
    VariablesBinairesEnCours = OUI_MPS; 
    continue;
  }

  if ( strstr( LigneLue , "MARKER" ) != 0 &&  strstr( LigneLue , "INTEND" ) != 0 ) {
    /* printf("Fin lecture de variables entieres \n"); */
    if( VariablesBinairesEnCours == OUI_MPS ) VariablesBinairesEnCours = NON_MPS; 
    continue;
  }

  NbChamps = sscanf( LigneLue , "%s %s %s %s %s" ,
                     LabelDeLaVariableLue    , LabelDeLaContrainteLue  , ValeurLue ,
                     LabelDeLaContrainteLueB , ValeurLueB );
  if ( NbChamps <= 0 ) continue; 
   
  CoefficientMps  = atof( ValeurLue );
  if ( NbChamps > 3 ) CoefficientMpsB = atof( ValeurLueB );

  if ( strcmp( LabelDeLaVariableLue , LabelDeLaVariablePrecedente ) != 0 ) { /* Nouvelle variable */
    problemMps->NbVar++;
    problemMps->LabelDeLaVariable[problemMps->NbVar] = (char *) malloc( strlen( LabelDeLaVariableLue ) + 1 ); 
    strcpy( problemMps->LabelDeLaVariable[problemMps->NbVar] , LabelDeLaVariableLue );

    problemMps->CoefsObjectif[problemMps->NbVar] = 0.; 

    problemMps->Umin          [problemMps->NbVar] = 0.;
    problemMps->Umax          [problemMps->NbVar] = PlusLinfiniBis;
    problemMps->TypeDeVariable[problemMps->NbVar] = REEL; 

    problemMps->TypeDeBorneDeLaVariable[problemMps->NbVar] = VARIABLE_BORNEE_INFERIEUREMENT;					

    if ( VariablesBinairesEnCours == OUI_MPS ) {
      problemMps->Umin          [problemMps->NbVar] = 0.;
      problemMps->Umax          [problemMps->NbVar] = 1.;
      problemMps->TypeDeVariable[problemMps->NbVar] = ENTIER;
    }  
  }
  strcpy( LabelDeLaVariablePrecedente , LabelDeLaVariableLue );

  if( problemMps->LabelDeLObjectif != NULL ) {
    if ( strcmp( LabelDeLaContrainteLue , problemMps->LabelDeLObjectif ) == 0 ) {
      problemMps->CoefsObjectif[problemMps->NbVar] = CoefficientMps; 
      if ( NbChamps <= 3 ) continue; /* Vers la fin du while */
      else goto DeuxiemeSerie;
    } 
  }
  /* Recherche de la contrainte */
   PNE_RechercheDuNumeroDeContrainteMPS( /*CntCourant, BaseCnt,*/ &Cnt, LabelDeLaContrainteLue );

  /* Contrainte trouvee */
  if ( problemMps->Mdeb[Cnt] < 0 ) {
    problemMps->Mdeb[Cnt]    = il + 1;
    problemMps->Msui[il + 1] = -1;
    problemMps->Mder[Cnt]    = il + 1;
  }
  problemMps->NbTerm[Cnt]++;
  il++;
  ilk            = problemMps->Mder[Cnt] ;
  problemMps->Msui [ilk] = il;
  problemMps->Msui [il]  = -1;
  problemMps->Mder [Cnt] = il;
  problemMps->A    [il]  = CoefficientMps;
  problemMps->Nuvar[il]  = problemMps->NbVar;

  CntCourant = Cnt;

  /* Nouvelle lecture sur la meme ligne */
  DeuxiemeSerie:

  if ( NbChamps <= 3 ) continue; 

  if( problemMps->LabelDeLObjectif != NULL ) {
    if ( strcmp( LabelDeLaContrainteLueB , problemMps->LabelDeLObjectif ) == 0 ) {
		  if ( problemMps->CoefsObjectif[problemMps->NbVar] != 0.0 ) {
			  if ( problemMps->CoefsObjectif[problemMps->NbVar] != CoefficientMpsB ) {
				  printf("Erreur dans le fichier MPS: la variable %s est donnee avec plusieurs couts differents\n",LabelDeLaVariableLue);
				  printf("                            seule la premiere valeur rencontree est utilisee\n");
			  }
			}		
      if ( problemMps->CoefsObjectif[problemMps->NbVar] == 0.0 ) problemMps->CoefsObjectif[problemMps->NbVar] = CoefficientMpsB; 
      continue; /* Vers la fin du while */
    } 
  }
  
  /* Recherche de la 2eme contrainte qui se trouve sur la meme ligne */
   PNE_RechercheDuNumeroDeContrainteMPS( /*CntCourant, BaseCnt,*/ &Cnt, LabelDeLaContrainteLueB );
  /* Contrainte trouvee */
  if ( problemMps->Mdeb[Cnt] < 0 ) {
    problemMps->Mdeb[Cnt]    = il + 1;
    problemMps->Msui[il + 1] = -1;
    problemMps->Mder[Cnt]    = il + 1;
  }
  problemMps->NbTerm[Cnt]++;
  il++;
  ilk            = problemMps->Mder[Cnt] ;
  problemMps->Msui [ilk] = il;
  problemMps->Msui [il]  = -1;
  problemMps->Mder [Cnt] = il;
  problemMps->A    [il]  = CoefficientMpsB;
  problemMps->Nuvar[il]  = problemMps->NbVar;

  CntCourant = Cnt;

}
problemMps->NbVar++; /* Important */
   
BaseVar = 0;
PNE_CreerHashCodeVariableMPS( /*&BaseVar*/ );
/*
printf("Fin de la lecture de COLUMNS, nombre de variables %d \n",problemMps->NbVar); fflush(stdout);    
*/
/*           Lecture des valeurs de second membre            */
/* The section is preceded by a card with RHS in columns 1-3 */
NbInit     = 0;
CntCourant = 0;

while ( 1 ) {
  fgets( LigneLue , LgMax , Flot ); 
  if( strstr( LigneLue , "RANGES" ) == LigneLue ) break;
  if( strstr( LigneLue , "BOUNDS" ) == LigneLue ) goto BOUNDS;
  if( strstr( LigneLue , "ENDATA" ) == LigneLue ) goto FinLecture;

  NbChamps = sscanf( LigneLue , "%s %s %s %s %s" ,
                     LabelDuSecondMembreLu  , LabelDeLaContrainteLue  , ValeurLue ,
                     LabelDeLaContrainteLueB , ValeurLueB ); 
  if ( NbChamps <= 0 ) continue; 
	
  CoefficientMps  = atof( ValeurLue );
  if ( NbChamps > 3 ) CoefficientMpsB = atof( ValeurLueB );

  PNE_RechercheDuNumeroDeContrainteMPS( /*CntCourant, BaseCnt,*/ &Cnt, LabelDeLaContrainteLue );	

  /* Contrainte trouvee */
  NbInit++;
  problemMps->LabelDuSecondMembre[Cnt] = (char *) malloc( strlen( LabelDuSecondMembreLu ) + 1 );
  strcpy( problemMps->LabelDuSecondMembre[Cnt] , LabelDuSecondMembreLu );
  problemMps->Rhs[Cnt] = CoefficientMps;

  CntCourant = Cnt;

  /* Nouvelle lecture sur la meme ligne */
  if ( NbChamps <= 3 ) continue; 

  /* Recherche de la 2eme contrainte qui se trouve sur la meme ligne */

    PNE_RechercheDuNumeroDeContrainteMPS( /*CntCourant, BaseCnt,*/ &Cnt, LabelDeLaContrainteLueB );

    /* Contrainte trouvee */
    NbInit++;
    problemMps->LabelDuSecondMembre[Cnt] = (char *) malloc( strlen( LabelDuSecondMembreLu ) + 1 );
    strcpy( problemMps->LabelDuSecondMembre[Cnt] , LabelDuSecondMembreLu ); /* A revoir a corriger car ca sert a rien */
    problemMps->Rhs[Cnt] = CoefficientMpsB;

    CntCourant = Cnt;

}

/*           Presence eventuelle de contraintes Range        */

CntCourant = 0;
while ( 1 ) {
  fgets( LigneLue , LgMax , Flot ); 
  if( strstr( LigneLue , "BOUNDS" ) == LigneLue ) break;
  if( strstr( LigneLue , "ENDATA" ) == LigneLue ) goto FinLecture;

  NbChamps = sscanf( LigneLue , "%s %s %s %s %s" ,
                     LabelDuSecondMembreLu  , LabelDeLaContrainteLue  , ValeurLue ,
                     LabelDeLaContrainteLueB , ValeurLueB ); 
  if ( NbChamps <= 0 ) continue; 
	
  CoefficientMps  = atof( ValeurLue );
  if ( NbChamps > 3 ) CoefficientMpsB = atof( ValeurLueB );

  PNE_RechercheDuNumeroDeContrainteMPS( /*CntCourant, BaseCnt,*/ &Cnt, LabelDeLaContrainteLue );	

  /* Contrainte trouvee */
	problemMps->NbCntRange+= 1;
  problemMps->BRange[Cnt] = CoefficientMps;

  CntCourant = Cnt;

  /* Nouvelle lecture sur la meme ligne */
  if ( NbChamps <= 3 ) continue; 

  /* Recherche de la 2eme contrainte qui se trouve sur la meme ligne */

    PNE_RechercheDuNumeroDeContrainteMPS( /*CntCourant, BaseCnt,*/ &Cnt, LabelDeLaContrainteLueB );

    /* Contrainte trouvee */
		problemMps->NbCntRange+= 1;
    problemMps->BRange[Cnt] = CoefficientMpsB;

    CntCourant = Cnt;

}

BOUNDS:

/*
              Lecture des bornes sur le variables              
  The section is preceded by a card with BOUNDS in columns 1-6.  
  The bounds are entered as a row, with a corresponding row label. 
  The nonzero entries in this row vector correspond to columns 
  in the matrix and must be in the same order in which the column 
  names appear in the COLUMNS section. 
  When bounds are not specified for a column (or the entire BOUNDS 
  section omitted) the usual bounds 0 + infini , are assumed. 
  More than one bound for a particular variable may be entered, i.e., 
  both a lower and an upper bound; when only one is specified the 
  other is assumed to be one of the default values of 0 or +infini 
  as shown in parentheses below. 

 Field 1 (columns 2-3) specifies the type of bound:
 LO lower bound
 UP upper bound
 LI lower bound integer variable
 UI upper bound integer variable
 BV binary variable
 FX fixed variable
 FR free 
 MI lower bound - infini
 PL upper bound + infini */

VarCourant = 0;

while ( 1 ) {

  fgets( LigneLue , LgMax , Flot );
  if( strstr( LigneLue , "ENDATA" ) == LigneLue ) goto FinLecture;

  NbChamps = sscanf( LigneLue , "%s %s %s %s" ,
             TypeDeBorne , LabelDeLaBorneLue  , LabelDeLaVariableLue , ValeurLue ); 
  if ( NbChamps <= 0 ) continue;
   
  CoefficientMps  = atof( ValeurLue );
  /*CoefficientMpsB = atof( ValeurLueB );*/

  PNE_RechercheDuNumeroDeVariableMPS( /*VarCourant, BaseVar,*/ &i, LabelDeLaVariableLue );
	
	if ( i < 0 ) {
    /* Creation d'une variable avec un cout nul */
		i = problemMps->NbVar;
		printf("Creation de la variable num %d\n",i);
		problemMps->NbVar++;
    problemMps->LabelDeLaVariable       = (char **)  realloc( problemMps->LabelDeLaVariable      , problemMps->NbVar * sizeof( void * ) );
    problemMps->TypeDeVariable          = (int *)    realloc( problemMps->TypeDeVariable         , problemMps->NbVar * sizeof( int    ) );
    problemMps->TypeDeBorneDeLaVariable = (int *)    realloc( problemMps->TypeDeBorneDeLaVariable, problemMps->NbVar * sizeof( int    ) );
    problemMps->U                       = (double *) realloc( problemMps->U                      , problemMps->NbVar * sizeof( double ) );
    problemMps->CoefsObjectif                       = (double *) realloc( problemMps->CoefsObjectif                      , problemMps->NbVar * sizeof( double ) );
    problemMps->Umin                    = (double *) realloc( problemMps->Umin                   , problemMps->NbVar * sizeof( double ) );
    problemMps->Umax                    = (double *) realloc( problemMps->Umax                   , problemMps->NbVar * sizeof( double ) );
    if ( problemMps->LabelDeLaVariable    == NULL || problemMps->TypeDeVariable == NULL || problemMps->TypeDeBorneDeLaVariable == NULL || 
         problemMps->U                    == NULL || problemMps->CoefsObjectif              == NULL || problemMps->Umin                    == NULL ||
         problemMps->Umax                 == NULL ) {
      printf("PNE memoire insuffisante dans le sous programme PNE_LireJeuDeDonneesMPS \n");
      exit(0);
    }

    problemMps->LabelDeLaVariable[i] = (char *) malloc( strlen( LabelDeLaVariableLue ) + 1 ); 
    strcpy( problemMps->LabelDeLaVariable[i] , LabelDeLaVariableLue );

    problemMps->CoefsObjectif[i] = 0.; 
    /* Initialisations par defaut */
    problemMps->Umin          [i] = 0.;
    problemMps->Umax          [i] = PlusLinfiniBis;
    problemMps->TypeDeVariable[i] = REEL; 
    problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_INFERIEUREMENT;					 
    /* Pas de hashcode pour la variable */
		
	}
	
  if ( strcmp( TypeDeBorne , "LO" ) == 0 ) {
    /* Borne inferieure */
    problemMps->Umin[i] = CoefficientMps;

    if ( problemMps->Umin[i] > problemMps->Umax[i] ) { printf("Erreur de donnees \n"); exit(0); }

    if ( problemMps->TypeDeVariable[i] != ENTIER ) {	/* Si le type n'est pas deja defini par INTORG */
      problemMps->TypeDeVariable[i] = REEL;
    }
    if ( problemMps->TypeDeBorneDeLaVariable[i] == NON_DEFINI ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_INFERIEUREMENT;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_NON_BORNEE ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_INFERIEUREMENT;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_SUPERIEUREMENT ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_DES_DEUX_COTES;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_INFERIEUREMENT ) {
      /* C'est une redefinition de la borne */
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_DES_DEUX_COTES ) {
      /* C'est une redefinition de la borne */
    }
    else {
      printf("Lecture des donnees, impossible d'affecter un type de borne a la variable %d TypeDeBorne %s Type existant %d\n"
             ,i,TypeDeBorne,problemMps->TypeDeBorneDeLaVariable[i]);
      exit(0);
    }
    continue;  
  }
  if ( strcmp( TypeDeBorne , "UP" ) == 0 ) {        
    problemMps->Umax[i] = CoefficientMps;
    if ( problemMps->TypeDeVariable[i] != ENTIER ) { /* Si le type n'est pas deja defini par INTORG */
      problemMps->TypeDeVariable[i] = REEL;
    }
    if ( problemMps->TypeDeBorneDeLaVariable[i] == NON_DEFINI ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_SUPERIEUREMENT;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_NON_BORNEE ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_SUPERIEUREMENT;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_INFERIEUREMENT ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_DES_DEUX_COTES;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_SUPERIEUREMENT ) {
      /* C'est une redefinition de la borne */
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_DES_DEUX_COTES ) {
      /* C'est une redefinition de la borne */
    }   
    else {    
      printf("Lecture des donnees, impossible d'affecter un type de borne a la variable %d TypeDeBorne %s TypeCourant %d\n",
              i,TypeDeBorne,problemMps->TypeDeBorneDeLaVariable[i]);
      exit(0);    
    }
    continue;  
  }
  if ( strcmp( TypeDeBorne , "MI" ) == 0 ) {
    if ( problemMps->TypeDeVariable[i] != ENTIER ) { /* Si le type n'est pas deja defini par INTORG */
      problemMps->Umin                   [i] = -PlusLinfiniBis;
      problemMps->Umax                   [i] = 0.;
      problemMps->U                      [i] = 0.;
      problemMps->TypeDeVariable         [i] = REEL;
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_SUPERIEUREMENT;	
      continue;  
    }
    else { 
      printf(" L'optimisation avec des variables entieres comprises entre 0 et - l'infini n'est pas implémentée\n");
      exit(0);
    }
  }
  if ( strcmp( TypeDeBorne , "FR" ) == 0 ) {
    problemMps->Umin                   [i] = -PlusLinfiniBis;
    problemMps->Umax                   [i] =  PlusLinfiniBis;
    problemMps->TypeDeVariable         [i] = REEL;
    problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_NON_BORNEE;
    continue;  
  }
  if ( strcmp( TypeDeBorne , "LI" ) == 0 ) {
    problemMps->Umin          [i] = CoefficientMps;
    problemMps->TypeDeVariable[i] = ENTIER;  
    if ( problemMps->TypeDeBorneDeLaVariable[i] == NON_DEFINI ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_INFERIEUREMENT;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_NON_BORNEE ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_INFERIEUREMENT;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_SUPERIEUREMENT ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_DES_DEUX_COTES;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_INFERIEUREMENT ) {
      /* C'est une redefinition de la borne */
    }
    else {
      printf("Lecture des donnees, impossible d'affecter un type de borne a la variable %d TypeDeBorne %s\n",i,TypeDeBorne);
      exit(0);
    }
    continue;  
  }
  if ( strcmp( TypeDeBorne , "UI" ) == 0 ) {
    problemMps->Umax          [i] = CoefficientMps;
    problemMps->TypeDeVariable[i] = ENTIER;
    if ( problemMps->TypeDeBorneDeLaVariable[i] == NON_DEFINI ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_SUPERIEUREMENT;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_NON_BORNEE ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_SUPERIEUREMENT;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_INFERIEUREMENT ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_DES_DEUX_COTES;
    }
    else if ( problemMps->TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_SUPERIEUREMENT ) {
      /* C'est une redefinition de la borne */
    }
    else {
      printf("Lecture des donnees, impossible d'affecter un type de borne a la variable %d TypeDeBorne %s\n",i,TypeDeBorne);
      exit(0);
    }
    continue;  
  }
  if ( strcmp( TypeDeBorne , "BV" ) == 0 ) {	
    problemMps->Umin                   [i] = 0.;    	     
    problemMps->Umax                   [i] = 1.;
    problemMps->TypeDeVariable         [i] = ENTIER;  
    problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_DES_DEUX_COTES;	
    continue;  
  }
  if ( strcmp( TypeDeBorne , "SC" ) == 0 ) {
    problemMps->Umin                   [i] = 0.;
    problemMps->Umax                   [i] = 1.;
    problemMps->TypeDeVariable         [i] = ENTIER;
    problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_DES_DEUX_COTES;	
    continue;  
  }
  if ( strcmp( TypeDeBorne , "FX" ) == 0 ) {
    problemMps->Umin                   [i] = CoefficientMps;
    problemMps->Umax                   [i] = CoefficientMps;
    problemMps->U                      [i] = CoefficientMps;
    problemMps->TypeDeVariable         [i] = REEL;
    problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_FIXE;		
    continue;  
  }
  if ( strcmp( TypeDeBorne , "PL" ) == 0 ) {
	  /* Borne sup infinie => rien a faire */
		continue;
	}
  printf(" Code borne inconnu %s\n",TypeDeBorne); exit(0);

}

/*
printf("Fin de lecture de BOUNDS \n"); fflush(stdout); 
*/

FinLecture:

for ( i = 0 ; i < problemMps->NbVar ; i++ ) {
  if ( problemMps->TypeDeVariable[i] == ENTIER ) {
    if ( fabs( problemMps->Umax[i] - problemMps->Umin[i] ) < PlusLinfiniBis ) {
      problemMps->TypeDeBorneDeLaVariable[i] = VARIABLE_BORNEE_DES_DEUX_COTES;					      
    }  
  }
}

/* On enleve les contraintes de type Free */
/* Algorithme naif ou l'on espere qu'il n'y a pas beaucoup de contraintes Free */
for ( i = 0 ; i < problemMps->NbCnt ; i++ ) {
  if ( problemMps->SensDeLaContrainte[i] == 'N' ) {
    /* Contrainte libre: on peut suprimer */
    for ( j = i ; j < problemMps->NbCnt-1 ; j++ ) {		  
      problemMps->Mdeb[j] = problemMps->Mdeb[j+1];
      problemMps->NbTerm[j] = problemMps->NbTerm[j+1];
			problemMps->SensDeLaContrainte[j] = problemMps->SensDeLaContrainte[j+1];
			problemMps->Rhs[j] = problemMps->Rhs[j+1];
			problemMps->BRange[j] = problemMps->BRange[j+1];
			problemMps->LabelDeLaContrainte[j] = problemMps->LabelDeLaContrainte[j+1];
		}
		problemMps->NbCnt--;
		i--;
	}
}

/*            Classer les contraintes            */
/* printf("Classement des contraintes\n"); */

A      = (double *) malloc( CountTrm   * sizeof( double ) );
Nuvar  = (int *)   malloc( CountTrm   * sizeof( int   ) );
Mdeb   = (int *)   malloc( problemMps->NbCnt  * sizeof( int   ) );
NbTerm = (int *)   malloc( problemMps->NbCnt  * sizeof( int   ) );
if ( A == NULL || Nuvar == NULL || Mdeb == NULL || NbTerm == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_LireJeuDeDonneesMPS \n");
  exit(0);
}

il = -1;
for ( i = 0 ; i < problemMps->NbCnt ; i++ ) {
  Mdeb  [i] = il + 1;
  NbTerm[i] = problemMps->NbTerm[i];
  ilMps     = problemMps->Mdeb[i];
  while ( ilMps >= 0 ) {
    il++;
    A    [il] = problemMps->A    [ilMps];
    Nuvar[il] = problemMps->Nuvar[ilMps];   
    ilMps = problemMps->Msui[ilMps];
  }
}

memcpy( (char *) problemMps->A      , (char *) A      , CountTrm   * sizeof( double ) );
memcpy( (char *) problemMps->Nuvar  , (char *) Nuvar  , CountTrm   * sizeof( int   ) );
memcpy( (char *) problemMps->Mdeb   , (char *) Mdeb   , problemMps->NbCnt  * sizeof( int   ) );
memcpy( (char *) problemMps->NbTerm , (char *) NbTerm , problemMps->NbCnt  * sizeof( int   ) );

free( A ); free( Nuvar ) ; free( Mdeb ) ; free( NbTerm );

/* Classement des variables dans les contraintes */
PNE_ClasserVariablesDansContraintesMPS();

/* Ajustement des couts pour tenir compte du sens de l'optimisation */

if ( SensOpt == 1.0 ) printf("Minimization\n");
else if ( SensOpt == -1.0 ) printf("Maximization\n");
else {
  printf("Sens de l'optimisation Minimisation ou Maximisation non defini\n");
	exit(0);
}

for ( i = 0 ; i < problemMps->NbVar ; i++ ) {
  problemMps->CoefsObjectif[i] *= SensOpt;
}

printf("Reading MPS file -> rows: %d   columns: %d \n",problemMps->NbCnt,problemMps->NbVar);

free( LigneLue );
free( LabelDeLaContrainteLue );	  
free( LabelDeLaVariableLue );
free( LabelDeLaContrainteLueB );	  
free( LabelDuSecondMembreLu );	  
free( LabelDeLaBorneLue );
free( LabelDeLaVariablePrecedente );
free( ValeurLue );
free( ValeurLueB );
free( ValeurLueC );

/* On libere le flot et le buffer */
fclose( Flot );

free( problemMps->FirstNomCnt );
free( problemMps->NomCntSuivant );
free( problemMps->FirstNomVar );
free( problemMps->NomVarSuivant );

if ( problemMps->NbCntRange > 0 ) {
  printf("  Nombre de Ranges %d  !!!!!\n",problemMps->NbCntRange);
  PNE_PrendreEnCompteLesContraintesRangeMPS();
}

return;	  

}

/*----------------------------------------------------------------------------*/
/*
      Pour chaque contrainte, on classe les variables dans l'ordre croissant
      de leur numero 
*/

void PNE_ClasserVariablesDansContraintesMPS()
{
int i ; int j; int iX; int Prem; int PremSv; int Lp; int Lz;

double * TabSv; int * Suiv;

/* Inutile car c'est fait dans PNE Solveur */
return;

TabSv  = (double *)   malloc( Mps.NbVar * sizeof( double  ) );
Suiv   = (int *)     malloc( Mps.NbVar * sizeof( int   ) );
if ( TabSv == NULL || Suiv == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_LireJeuDeDonneesMPS \n");
  exit(0);
}

for ( j = 0 ; j < Mps.NbCnt ; j++ ) {

  Prem = -1;
  for ( i = Mps.Mdeb[j] ; i <= Mps.Mdeb[j] + Mps.NbTerm[j] - 1 ; i++ ) {
    iX        = Mps.Nuvar[i];
    TabSv[iX] = Mps.A[i];
    Lp = -1;
    Lz = Prem;
    while ( 1 ) {
      if ( Lz < 0 || iX < Lz ) {
        if ( Lp < 0 ) {
          /* Premier terme */
          PremSv   = Prem;
          Prem     = iX;
          Suiv[iX] = PremSv;
          break;
        }
        /* Lp >= 0 */
        Suiv[Lp] = iX;
        Suiv[iX] = Lz;
        break;
      }  
      Lp = Lz;
      Lz = Suiv[Lz];
    }  
  }

  /* Reclassement dans l ordre */
  i  = Mps.Mdeb[j];
  Lz = Prem;
  while ( Lz >= 0 ) {
    Mps.Nuvar[i] = Lz;
    Mps.A    [i] = TabSv[Lz];
    i++;
    Lz = Suiv[Lz];
  }

}

free( TabSv );
free( Suiv );

return;
}

/*----------------------------------------------------------------------------------------*/

void PNE_RechercheDuNumeroDeContrainteMPS( /*int   CntCourant,*/
                                           /*int   BaseCnt,*/ 
                                           int * CntTrouv,
                                           char * LabelDeLaContrainteLue )
{
int Cnt; char * pt1; int k;  

/* Recherche de la contrainte */

pt1 = LabelDeLaContrainteLue;

/* Hashcode du Nom */
k = PNE_HashCode( pt1 , Mps.NentreesCnt );

Cnt = Mps.FirstNomCnt[k];
while ( Cnt >= 0 ) {
  if ( strcmp( Mps.LabelDeLaContrainte[Cnt] , LabelDeLaContrainteLue ) == 0 ) {
    *CntTrouv = Cnt;
    return;
  }
  Cnt = Mps.NomCntSuivant[Cnt];
}

printf(" Contrainte inconnue -%s- HashCode %d \n",LabelDeLaContrainteLue,k);
exit(0);

return;
}

/*----------------------------------------------------------------------------------------*/

void PNE_RechercheDuNumeroDeVariableMPS( /*int   VarCourant, 
                                         int   BaseVar,*/ 
                                         int * VarTrouv,
                                         char * LabelDeLaVariableLue )
{
int Var; char * pt1; int k;  

/* Recherche de la variable */
 
pt1 = LabelDeLaVariableLue;

/* Hashcode du Nom */
k = PNE_HashCode( pt1 , Mps.NentreesVar );  

Var = Mps.FirstNomVar[k];
while ( Var >= 0 ) {
  if ( strcmp( Mps.LabelDeLaVariable[Var] , LabelDeLaVariableLue ) == 0 ) {
    *VarTrouv = Var;
    return;
  }
  Var = Mps.NomVarSuivant[Var];
}

 printf(" Variable inconnue %s , on la cree avec un cout nul\n",LabelDeLaVariableLue);
*VarTrouv = -1;
 
return;
}

/*----------------------------------------------------------------------------------------*/

void PNE_CreerHashCodeContrainteMPS( /*int * BaseCnt*/ )
{
int Cnt; int Prem; char * pt1; int k; 

#if VERBOSE_PNE
  printf("Debut creation haschcode cnt\n"); fflush(stdout);
#endif

Mps.NentreesCnt = (int) pow( 2 , 20 );
 
Mps.FirstNomCnt   = (int *) malloc( Mps.NentreesCnt * sizeof( int ) );
Mps.NomCntSuivant = (int *) malloc( Mps.NbCnt       * sizeof( int ) );
if ( Mps.FirstNomCnt == NULL || Mps.NomCntSuivant == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_CreerHashCodeContrainteMPS \n");
  exit(0);
}
   
for ( k = 0 ; k < Mps.NentreesCnt ; k++ ) Mps.FirstNomCnt[k] = -1;

for ( Cnt = 0 ; Cnt < Mps.NbCnt ; Cnt++ ) {
  pt1 = Mps.LabelDeLaContrainte[Cnt];
  /* Hashcode du Nom */
  k = PNE_HashCode( pt1 , Mps.NentreesCnt );

  Prem = Mps.FirstNomCnt[k];
  Mps.FirstNomCnt  [k] = Cnt;
  Mps.NomCntSuivant[Cnt] = Prem;
}

#if VERBOSE_PNE
  printf("Fin creation haschcode cnt\n"); fflush(stdout);	   
#endif

return;
}

/*----------------------------------------------------------------------------------------*/

void PNE_CreerHashCodeVariableMPS( /*int * BaseVar*/ )
{
int Var; int Prem; char * pt1; int k; 

#if VERBOSE_PNE
  printf("Debut creation haschcode var\n"); fflush(stdout);
#endif

/*
Mps.NentreesVar = (int) ( 5 * Mps.NbVar ) + 1;
*/
Mps.NentreesVar = (int) pow( 2 , 20 );
 
Mps.FirstNomVar   = (int *) malloc( Mps.NentreesVar * sizeof( int ) );
Mps.NomVarSuivant = (int *) malloc( Mps.NbVar       * sizeof( int ) );
if ( Mps.FirstNomVar == NULL || Mps.NomVarSuivant == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_CreerHashCodeVariableMPS \n");
  exit(0);
}

for ( k = 0 ; k < Mps.NentreesVar ; k++ ) Mps.FirstNomVar[k] = -1;

for ( Var = 0 ; Var < Mps.NbVar ; Var++ ) {
  pt1 = Mps.LabelDeLaVariable[Var];
  /* Hashcode du Nom */
  k = PNE_HashCode( pt1 , Mps.NentreesVar );

  Prem = Mps.FirstNomVar[k];
  Mps.FirstNomVar  [k]   = Var;
  Mps.NomVarSuivant[Var] = Prem;
}

#if VERBOSE_PNE
  printf("Fin creation haschcode var\n"); fflush(stdout);
#endif

return;
}

/*----------------------------------------------------------------------------------------*/

int PNE_HashCode( char * pt1 , int Nentrees )
{
int Int; char Car; int k;

/* Hashcode du Nom */
k = kHashCode;
while ( sscanf( pt1 , "%c", &Car ) > 0 ) { 
  Int = (int) Car;
  pt1++; 
  k = ( k * aHashCode + Int ) % Nentrees;
}
return( k );

}

/***************************************************************************/

void PNE_PrendreEnCompteLesContraintesRangeMPS()
{
int Cnt; int NewNbCnt; int NbTermesEnPlus; int NewNbTermes; int NbVarNew;
int CntCourant; int il; int ilMax; int ilCourant; double Uinf; double Usup;
int * Mdeb_New; int * NbTerm_New; int * Nuvar_New; double * A_New;
char MethodeAvecVariableSupplementaire;

MethodeAvecVariableSupplementaire = OUI_MPS;

if ( MethodeAvecVariableSupplementaire == OUI_MPS ) goto METHODE_AVEC_VARIABLE_SUPPLEMENTAIRE;

NewNbCnt = Mps.NbCnt + Mps.NbCntRange;

/* On compte le nombre de termes supplementaires a mettre dans la matrice des contraintes */
NbTermesEnPlus = 0;
for ( Cnt = 0 ; Cnt < Mps.NbCnt ; Cnt++ ) {
  if ( Mps.BRange[Cnt] != 0.0 ) NbTermesEnPlus+= Mps.NbTerm[Cnt];
}

NewNbTermes = Mps.Mdeb[Mps.NbCnt - 1] + Mps.NbTerm[Mps.NbCnt - 1] + NbTermesEnPlus;

/* Reallocations */
Mps.Mdeb                = (int *) realloc( Mps.Mdeb,   NewNbCnt * sizeof( int ) );
Mps.NbTerm              = (int *) realloc( Mps.NbTerm, NewNbCnt * sizeof( int ) );
Mps.Rhs                   = (double *) realloc( Mps.Rhs                 , NewNbCnt * sizeof( double ) );
Mps.SensDeLaContrainte  = (char *)   realloc( Mps.SensDeLaContrainte, NewNbCnt * sizeof( char   ) );

Mps.VariablesDualesDesContraintes = (double *) realloc( Mps.VariablesDualesDesContraintes, NewNbCnt * sizeof( double ) );
Mps.LabelDeLaContrainte           = (char **)  realloc( Mps.LabelDeLaContrainte          , NewNbCnt  * sizeof( void * ) );

if ( Mps.Mdeb                          == NULL || Mps.NbTerm              == NULL ||
     Mps.Rhs                             == NULL || Mps.SensDeLaContrainte  == NULL ||
		 Mps.VariablesDualesDesContraintes == NULL || Mps.LabelDeLaContrainte == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_PrendreEnCompteLesContraintesRangeMPS \n");
  exit(0);
}

Mps.A     = (double *) realloc( Mps.A    , NewNbTermes * sizeof( double ) );
Mps.Nuvar = (int *)   realloc( Mps.Nuvar, NewNbTermes * sizeof( int   ) );

CntCourant = Mps.NbCnt;
ilCourant  = Mps.Mdeb[Mps.NbCnt - 1] + Mps.NbTerm[Mps.NbCnt - 1];
for ( Cnt = 0 ; Cnt < Mps.NbCnt ; Cnt++ ) {
  if ( Mps.BRange[Cnt] == 0.0 ) continue;
  il    = Mps.Mdeb[Cnt];
	ilMax = il + Mps.NbTerm[Cnt];
	Mps.Mdeb[CntCourant] = ilCourant;
	while ( il < ilMax ) {
	  Mps.A[ilCourant] = Mps.A[il];
	  Mps.Nuvar[ilCourant] = Mps.Nuvar[il];
    ilCourant++;
	  il++;
	}
	Mps.NbTerm[CntCourant] = ilCourant - Mps.Mdeb[CntCourant];
	if ( Mps.SensDeLaContrainte[Cnt] == '<' ) {
	  Mps.Rhs[CntCourant] = Mps.Rhs[Cnt] - fabs( Mps.BRange[Cnt] );
		Mps.SensDeLaContrainte[CntCourant] = '>';
	}
	else if ( Mps.SensDeLaContrainte[Cnt] == '>' ) {
	  Mps.Rhs[CntCourant] = Mps.Rhs[Cnt] + fabs( Mps.BRange[Cnt] );
		Mps.SensDeLaContrainte[CntCourant] = '<';
	}
	else {
    if ( Mps.BRange[Cnt] > 0.0 ) {
      Mps.SensDeLaContrainte[Cnt] = '>';
	    Mps.Rhs[CntCourant] = Mps.Rhs[Cnt] + fabs( Mps.BRange[Cnt] );
		  Mps.SensDeLaContrainte[CntCourant] = '<';
		}
		else {
      Mps.SensDeLaContrainte[Cnt] = '<';
	    Mps.Rhs[CntCourant] = Mps.Rhs[Cnt] - fabs( Mps.BRange[Cnt] );
	  	Mps.SensDeLaContrainte[CntCourant] = '>';
		}		
	}
  CntCourant++;	
}

Mps.NbCnt = NewNbCnt;

free( Mps.BRange );

return;

METHODE_AVEC_VARIABLE_SUPPLEMENTAIRE:

/* Methode 2 on cree une contrainte d'egalite avec une variable bornee */

NbVarNew = Mps.NbVar + Mps.NbCntRange;

Mps.LabelDeLaVariable       = (char **)  realloc( Mps.LabelDeLaVariable      , NbVarNew * sizeof( void * ) );
Mps.TypeDeVariable          = (int *)   realloc( Mps.TypeDeVariable         , NbVarNew * sizeof( int          ) );
Mps.TypeDeBorneDeLaVariable = (int *)   realloc( Mps.TypeDeBorneDeLaVariable, NbVarNew * sizeof( int          ) );
Mps.U                       = (double *) realloc( Mps.U                      , NbVarNew * sizeof( double        ) );
Mps.CoefsObjectif                       = (double *) realloc( Mps.CoefsObjectif                      , NbVarNew * sizeof( double        ) );
Mps.Umin                    = (double *) realloc( Mps.Umin                   , NbVarNew * sizeof( double        ) );
Mps.Umax                    = (double *) realloc( Mps.Umax                   , NbVarNew * sizeof( double        ) );
if ( Mps.LabelDeLaVariable == NULL || Mps.TypeDeVariable == NULL || Mps.TypeDeBorneDeLaVariable == NULL ||
     Mps.U                 == NULL || Mps.CoefsObjectif              == NULL || Mps.Umin                    == NULL ||
		 Mps.Umax              == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_PrendreEnCompteLesContraintesRangeMPS \n");
  exit(0);
}

Mdeb_New   = (int *) malloc( Mps.NbCnt * sizeof( int ) );
NbTerm_New = (int *) malloc( Mps.NbCnt * sizeof( int ) );
if ( Mdeb_New == NULL || NbTerm_New == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_PrendreEnCompteLesContraintesRangeMPS \n");
  exit(0);
}

NbTermesEnPlus = Mps.NbCntRange;
NewNbTermes = Mps.Mdeb[Mps.NbCnt - 1] + Mps.NbTerm[Mps.NbCnt - 1] + NbTermesEnPlus;
A_New     = (double *) malloc( NewNbTermes * sizeof( double ) );
Nuvar_New = (int *)   malloc( NewNbTermes * sizeof( int   ) );
if ( A_New == NULL || Nuvar_New == NULL ) {
  printf("PNE memoire insuffisante dans le sous programme PNE_PrendreEnCompteLesContraintesRangeMPS \n");
  exit(0);
}

ilCourant = 0;
for ( Cnt = 0 ; Cnt < Mps.NbCnt ; Cnt++ ) {
	/* On recopie la contrainte */
  il    = Mps.Mdeb[Cnt];
	ilMax = il + Mps.NbTerm[Cnt];	
	Mdeb_New[Cnt] = ilCourant;
	NbTerm_New[Cnt] = Mps.NbTerm[Cnt];
	while ( il < ilMax ) {
	  A_New[ilCourant]     = Mps.A[il];
	  Nuvar_New[ilCourant] = Mps.Nuvar[il];
    ilCourant++;
	  il++;
	}		
  if ( Mps.BRange[Cnt] == 0.0 ) continue;	
	/* On ajoute un terme a la contrainte */
	A_New[ilCourant]     = -1.0;
	Nuvar_New[ilCourant] = Mps.NbVar;
  ilCourant++;
	NbTerm_New[Cnt]++;
	/* On transforme la contrainte en contrainte d'egalite */
	if ( Mps.SensDeLaContrainte[Cnt] == '<' ) {
	  Usup = Mps.Rhs[Cnt];
		Uinf = Usup - fabs( Mps.BRange[Cnt] );
	}
	else if ( Mps.SensDeLaContrainte[Cnt] == '>' ) {
	  Uinf = Mps.Rhs[Cnt];
    Usup = Uinf + fabs( Mps.BRange[Cnt] );
	}
	else {
		if ( Mps.BRange[Cnt] > 0.0 ) {
	    Uinf = Mps.Rhs[Cnt];
      Usup = Uinf + fabs( Mps.BRange[Cnt] );
		}
		else {
	    Usup = Mps.Rhs[Cnt];
		  Uinf = Usup - fabs( Mps.BRange[Cnt] );
		}		
	}
  Mps.Rhs[Cnt] = 0.0;
	Mps.SensDeLaContrainte[Cnt] = '=';	
	/* On cree la variable */
	Mps.TypeDeVariable         [Mps.NbVar] = REEL;
	Mps.TypeDeBorneDeLaVariable[Mps.NbVar] = VARIABLE_BORNEE_DES_DEUX_COTES;					      
  Mps.U   [Mps.NbVar] = Uinf;
  Mps.CoefsObjectif   [Mps.NbVar] = 0.0;
  Mps.Umin[Mps.NbVar] = Uinf;
  Mps.Umax[Mps.NbVar] = Usup;
	if ( Mps.NbVar > 1000000000 ) {
	  printf("Trop de variable a optimiser, limite 1000000000\n");
		exit(0);
	}
  Mps.LabelDeLaVariable[Mps.NbVar] = (char *) malloc( 12 );
  if ( Mps.LabelDeLaVariable[Mps.NbVar] == NULL ) {
    printf("PNE memoire insuffisante dans le sous programme PNE_PrendreEnCompteLesContraintesRangeMPS \n");
    exit(0);
  }	
  sprintf( Mps.LabelDeLaVariable[Mps.NbVar], "%d", Mps.NbVar);
	Mps.NbVar++;
}

free( Mps.Mdeb );
free( Mps.NbTerm );
free( Mps.A );
free( Mps.Nuvar );

Mps.Mdeb   = Mdeb_New;
Mps.NbTerm = NbTerm_New ;
Mps.A      = A_New ;
Mps.Nuvar  = Nuvar_New;

free( Mps.BRange );

return;
}
