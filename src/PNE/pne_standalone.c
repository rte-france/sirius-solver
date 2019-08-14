// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Main de la PNE pour le standalone
                
   AUTEUR: R. GONZALEZ

************************************************************************/
# define TEST_MULTI_THREADING
  # undef TEST_MULTI_THREADING

# ifdef TEST_MULTI_THREADING
  # include <pthread.h>
# endif

# define CPLUSPLUS
  # undef CPLUSPLUS

# include "pne_sys.h"
# include <stdio.h>
# include <ctype.h>

# ifdef __cplusplus
  # include "pne_definition_arguments.h"
  # include "pne_constantes_externes.h"	       						 
  # include "pne_define.h"
# else
  # include "pne_fonctions.h"
  # include "pne_constantes_externes.h"	       						 
  # include "pne_define.h"
# endif

# include "mps_define.h"
//# include "mps_extern_global.h"

# include "bb_define.h"

/*----------------------------------------------------------------------------*/
# ifdef TEST_MULTI_THREADING

void * StandAloneLancerUnThread( void * Arg )
{
  
  Probleme.NombreDeVariables       = Mps.NbVar;
  Probleme.TypeDeVariable          = Mps.TypeDeVariable;
  Probleme.TypeDeBorneDeLaVariable = Mps.TypeDeBorneDeLaVariable;
  Probleme.X                       = Mps.U;
  Probleme.Xmax                    = Mps.Umax;
  Probleme.Xmin                    = Mps.Umin;
  Probleme.CoutLineaire            = Mps.L;  
  Probleme.NombreDeContraintes                   = Mps.NbCnt;
  Probleme.SecondMembre                          = Mps.B;
  Probleme.Sens                                  = Mps.SensDeLaContrainte;
  Probleme.IndicesDebutDeLigne                   = Mps.Mdeb;
  Probleme.NombreDeTermesDesLignes               = Mps.NbTerm;
  Probleme.CoefficientsDeLaMatriceDesContraintes = Mps.A ;
  Probleme.IndicesColonnes                       = Mps.Nuvar;
  Probleme.VariablesDualesDesContraintes         = Mps.VariablesDualesDesContraintes;
  Probleme.SortirLesDonneesDuProbleme = NON_PNE;
  Probleme.AlgorithmeDeResolution     = SIMPLEXE; /* SIMPLEXE ou POINT_INTERIEUR */
  Probleme.CoupesLiftAndProject       = NON_PNE;
  Probleme.AffichageDesTraces         = OUI_PNE; 
  Probleme.FaireDuPresolve            = OUI_PNE;     
  if ( Probleme.FaireDuPresolve == NON_PNE ) printf("Attention pas de presolve\n");
  Probleme.TempsDExecutionMaximum       = 3600 * 0 /*3600 * 0*/;
  Probleme.NombreMaxDeSolutionsEntieres = -1;
  Probleme.ToleranceDOptimalite         = 1.e-4; /* C'est en % dont 1.e-4 ca fait 1.e-6 */
  
  PNE_Solveur( &Probleme );

	return( NULL );
}

# endif
/*----------------------------------------------------------------------------*/

void usage() {
	printf("\nUtilisation :\n\n");
	printf("ANTARES.exe -mps=\"fichier.mps\" [autres options]\n\n");
	printf("Options disponibles:\n");
	printf("  -mps=fichier    Chemin et nom du fichier au format mps a charger\n");
	printf("  -params=fichier Chemin et nom du fichier de parametres a charger\n");
	printf("  -gap=value      Tolerance d'optimalite pour la solution entiere (value est donne en pourcentage)\n");
	printf("  -printv         Pour afficher la valeur des variables en fin d'optimisation\n");
	printf("  -verif          Pour verifier que toutes les contraintes sont satisfaites par la solution trouvee\n");
	printf("  -all            Pour faire l'equivalent de -printv et -verif\n");
	printf("  -printp         Pour afficher la valeur des parameters en fin d'optimisation\n");
	printf("  -outp=fichier   Chemin et nom du fichier dans lequel ecrire les parametres en fin d'optimisation\n");
	printf("  -help ou -h     Pour obtenir la liste des options disponibles\n");
	printf("\n");
	exit(0);
}

int main( int argc , char ** argv ) {
if (argc == 1)
	usage();
int i ; int j; int YaUneSolution; double Critere; int Cnt; double S; 
int il; int ilMax; int CntMx; double Smx; double EcX; int Mip; double EcMoy; 
char AfficherLesValeursDesVariables; char AfficherLesValeursDesParametres;
char VerifierLesContraintes; char TestMultiThreading;
int Nbn; char * pt; char s[256]; char * pts; char ToleranceDOptimaliteExterne;
double ToleranceExterne;
FILE * FlotDeSortie;
char * nomFichierMps = "A_JEU_DE_DONNEES";
char * nomFichierInputParams = NULL; char * nomFichierOutputParams = NULL;

PROBLEME_A_RESOUDRE Probleme;
PROBLEME_MPS Mps;


# ifdef TEST_MULTI_THREADING
 int Erreur; pthread_t Thread[100]; int NbThread;   
# endif

AfficherLesValeursDesVariables = NON_PNE;
VerifierLesContraintes         = NON_PNE;
s[0] = '\0';
ToleranceDOptimaliteExterne = 0;
ToleranceExterne = 0.01;
for ( i = 1 ; i < argc ; i++ ) {
	if (strcmp(argv[i], "-help") == 0 || strcmp(argv[i], "-h") == 0)
		usage();
  if ( strcmp( argv[i] , "-printv" ) == 0 ) {
    AfficherLesValeursDesVariables = OUI_PNE;
    continue;  
  }
  if ( strcmp( argv[i] , "-verif" ) == 0 || strcmp( argv[i] , "-verfi" ) == 0 ) {
    VerifierLesContraintes = OUI_PNE;
    continue;
  }
  if ( strcmp( argv[i] , "-all" ) == 0 ) {
    AfficherLesValeursDesVariables = OUI_PNE;
    VerifierLesContraintes = OUI_PNE;
    continue;
  }
  if ( strstr( argv[i] , "-gap" ) != NULL ) {
		pt = strstr( argv[i] , "-gap" );
		pt = strstr( pt , "=" );
		if ( pt == NULL ) continue;
    pt++;		
		pts = s;		
		while ( isdigit( *pt ) ) {
		  *pts = *pt;  
			pts++;
			pt++;
		}
		/* On cherche la virgule */
		if ( *pt == '.' ) {
		  *pts = *pt;
			pts++;
			pt++;		  
		}
		while ( isdigit( *pt ) ) {
		  *pts = *pt;
			pts++;
			pt++;
		}		
		*pts = '\0';
		 
    ToleranceDOptimaliteExterne = 1;
    ToleranceExterne = atof( s );
		if ( ToleranceExterne < 0.0 ) {
		  printf("gap parameter must be > or = to 0\n");
			exit(0);
		}
    continue;
  }
  if (strstr(argv[i], "-mps") != NULL) {
	  pt = strstr(argv[i], "-mps");
	  pt = strstr(pt, "=");
	  if (pt == NULL) continue;
	  pt++;
	  nomFichierMps = pt;
	  continue;
  }
  if (strstr(argv[i], "-params") != NULL) {
	  pt = strstr(argv[i], "-params");
	  pt = strstr(pt, "=");
	  if (pt == NULL) continue;
	  pt++;
	  nomFichierInputParams = pt;
	  continue;
  }
  if (strcmp(argv[i], "-printp") == 0) {
	  AfficherLesValeursDesParametres = OUI_PNE;
	  continue;
  }
  if (strstr(argv[i], "-outp") != NULL) {
	  pt = strstr(argv[i], "-outp");
	  pt = strstr(pt, "=");
	  if (pt == NULL) continue;
	  pt++;
	  nomFichierOutputParams = pt;
	  continue;
  }
  printf("Option %s inconnue, taper l'option -help pour connaitre la liste des options disponibles\n",argv[i]);
  exit(0);
}

# ifdef _MSC_VER
  FlotDeSortie = NULL;
  fopen_s( &FlotDeSortie, "RESULTAT.csv", "w" ); 
# else
  FlotDeSortie = fopen( "RESULTAT.csv", "w" ); 
# endif

if( FlotDeSortie == NULL ) {
  printf("Erreur ouverture du fichier de resultats \n");
  exit(0);
}

/* Lecture du jeu de donnees */
/*
printf("Debut de la lecture des donnees \n"); fflush(stdout);
*/

PNE_LireJeuDeDonneesMPS(&Mps, nomFichierMps);

/*
printf("Fin de la lecture des donnees \n"); fflush(stdout);
*/
/* Mip: indicateur positionne a OUI_MPS si on veut optimiser en variables entieres 
        et positionne a NON_MPS si on veut juste faire un simplexe */

Mip = OUI_MPS;          	  			
if ( Mip == NON_MPS ) {    
  for ( j = 0 ; j < Mps.NbVar ; j++ ) Mps.TypeDeVariable[j] = REEL;
}

/*
printf("**********  Attention on fait une maximisation i.e Cout = -Cout ******************\n");
for ( j = 0 ; j < Mps.NbVar ; j++ ) Mps.L[j] = -Mps.L[j];
*/

goto PasDeBruitage;
{ double Cout; double S; double UnSurRAND_MAX;
  UnSurRAND_MAX = 1. / RAND_MAX;
  Cout = 1;
  for ( j = 0 ; j < Mps.NbVar ; j++ ){
	  /*if ( Mps.TypeDeVariable[j] == ENTIER ) {*/
		if ( Mps.CoefsObjectif[j] != 0.0 ) {
      S = (rand() * UnSurRAND_MAX ) - 0.5 ;
      S *= 0.9;
			Mps.CoefsObjectif[j] += S;  
			if ( Mps.CoefsObjectif[j] != 0 ) printf("Mps.L[%d] = %e\n",j,Mps.CoefsObjectif[j]);
		}
  }
}
PasDeBruitage:

/*
printf("Verification des bornes des variables\n");
for ( j = 0 ; j < Mps.NbVar ; j++ ) { 
  if ( Mps.Umin[j] > Mps.Umax[j] ) {printf("Erreur Umin %lf Umax %lf Var %d\n",Mps.Umin[j],Mps.Umax[j],j); exit(0);}
}
*/
/*
printf("Appel du solveur de PNE \n"); fflush(stdout);
*/

TestMultiThreading = NON_PNE;

# ifdef TEST_MULTI_THREADING
  if ( TestMultiThreading == OUI_PNE ) {
    NbThread = 1;
    i = 0;
    while ( i < 1 ) {
      for ( j = 0 ; j < NbThread ; j++ ) {
        Erreur = pthread_create( &Thread[j] , NULL , StandAloneLancerUnThread , NULL );
        if ( Erreur != 0 ) {
          printf("Erreur a la creation du thread\n"); exit(0);
        }
      }
      for ( j = 0 ; j < NbThread ; j++ ) {
        pthread_join( Thread[j] , NULL );
      }
      i++;
    }
    exit(0);
  }
# endif

PNE_PARAMS * pneParams = newDefaultPneParams();
if (nomFichierInputParams != NULL) {
	PNE_LireJeuDeParametres_AvecNom(pneParams, nomFichierInputParams, 1);
}

/* Resolution */
for ( j = 0 ; j < 1 ; j++ ) { /* Pour tester les fuites memoire on enchaine les resolutions du meme probleme */
  PNE_copy_problem(&Mps, &Probleme, ToleranceDOptimaliteExterne, ToleranceExterne);

  PNE_Solveur( &Probleme, pneParams );

  YaUneSolution = Probleme.ExistenceDUneSolution;

/*
for ( j = 0 ; j < Mps.NbVar ; j++ ){
  if ( Mps.TypeDeVariable[j] == ENTIER ) Mps.L[j] = 0.0;
}
*/

Nbn = 0;
printf("Dual variables count %d\n",Mps.NbCnt);
for ( Cnt = 0 ; Cnt < Mps.NbCnt ; Cnt++ ) {
  goto ABS;
	printf("Sense[%d] %c B = %e dual variable = %e\n",Cnt,Mps.SensDeLaContrainte[Cnt],Mps.Rhs[Cnt],
	        Mps.VariablesDualesDesContraintes[Cnt]);
	ABS:
  if ( fabs( Mps.VariablesDualesDesContraintes[Cnt] ) < 1.e-8 ) Nbn++;
}
printf("Zero dual variables count %d over %d\n",Nbn,Mps.NbCnt);

/*****************/

  if ( Probleme.AlgorithmeDeResolution == POINT_INTERIEUR ) goto FinTest;
  goto FinTest; 
  /* Test du local branching */
  { int LgL; int Var; double * TA; double K; int Cnt; int il;  

  Critere = 0.;
  for ( i = 0 ; i < Mps.NbVar ; i++ ) Critere+= Mps.CoefsObjectif[i] * Mps.U[i];
 
  if ( YaUneSolution == SOLUTION_OPTIMALE_TROUVEE ||  
       YaUneSolution == SOLUTION_OPTIMALE_TROUVEE_MAIS_QUELQUES_CONTRAINTES_SONT_VIOLEES ||  
       YaUneSolution == ARRET_PAR_LIMITE_DE_TEMPS_AVEC_SOLUTION_ADMISSIBLE_DISPONIBLE ) {
    printf("\n********** Objective %e **********\n\n",Critere);
  }
  
  LgL = -1;
  for ( Cnt = 0 ; Cnt < Mps.NbCnt ; Cnt++ ) {
    if ( Mps.Mdeb[Cnt] + Mps.NbTerm[Mps.NbCnt] > LgL ) {
      LgL = Mps.Mdeb[Cnt] + Mps.NbTerm[Mps.NbCnt];
    }
  }
  LgL+= Mps.NbVar;
  
  Mps.NbCnt++;
  Mps.Rhs                  = (double *) realloc( Mps.Rhs                  , Mps.NbCnt * sizeof( double ) );
  Mps.SensDeLaContrainte = ( char * ) realloc( Mps.SensDeLaContrainte , Mps.NbCnt * sizeof( char   ) );
  Mps.Mdeb               = ( int * ) realloc( Mps.Mdeb               , Mps.NbCnt * sizeof( int   ) );
  Mps.NbTerm             = ( int * ) realloc( Mps.NbTerm             , Mps.NbCnt * sizeof( int   ) );

  Mps.A     = (double *) realloc( Mps.A     , LgL * sizeof( double ) );
  Mps.Nuvar = (int *)   realloc( Mps.Nuvar , LgL * sizeof( int   ) );

  Mps.VariablesDualesDesContraintes = (double *) realloc( Mps.VariablesDualesDesContraintes , Mps.NbCnt * sizeof( double ) );

  TA = (double *) malloc( Mps.NbVar * sizeof( double ) );
  K = 1;
  Cnt = Mps.NbCnt - 1;
  Mps.Rhs[Cnt] = K;
  Mps.SensDeLaContrainte[Cnt] = '<';
  for ( Var = 0 ; Var < Mps.NbVar ; Var++ ) {
    TA[Var] = 0.0;
    if ( Mps.TypeDeVariable[Var] == ENTIER ) {
      TA[Var] = 1.;
      if ( Mps.U[Var] == 1.0 ) {
        Mps.Rhs[Cnt]-= 1;
        TA[Var] = -1.;
      } 
    }
  }
  il = Mps.Mdeb[Cnt - 1] + Mps.NbTerm[Cnt - 1];
  Mps.Mdeb[Cnt] = il;
  Mps.NbTerm[Cnt] = 0;
  for ( Var = 0 ; Var < Mps.NbVar ; Var++ ) {
    if ( TA[Var] != 0.0 ) {
      Mps.A[il] = TA[Var];
      Mps.Nuvar[il] = Var;
      Mps.NbTerm[Cnt]++;
      il++;
    }
  }
  /*printf("Mps.NbTerm[Cnt] %d\n",Mps.NbTerm[Cnt]);*/

  }  
  /* Fin Test du local branching */
  FinTest:
  continue;

}

/* Resultats */

if ( YaUneSolution == PAS_DE_SOLUTION_TROUVEE ) printf("No solution found\n"); 
if ( YaUneSolution == PROBLEME_INFAISABLE ) printf("Problem is infeasible\n"); 
if ( YaUneSolution == PROBLEME_NON_BORNE ) printf("Problem is unbounded\n"); 
 
if ( YaUneSolution == ARRET_CAR_ERREUR_INTERNE ) printf("Internal error\n"); 
if ( YaUneSolution ==  SOLUTION_OPTIMALE_TROUVEE_MAIS_QUELQUES_CONTRAINTES_SONT_VIOLEES ) 
printf("An optimal solution was found but some constraints remain violated\n"); 

if ( YaUneSolution == SOLUTION_OPTIMALE_TROUVEE || 
     YaUneSolution == ARRET_PAR_LIMITE_DE_TEMPS_AVEC_SOLUTION_ADMISSIBLE_DISPONIBLE ||
     YaUneSolution == SOLUTION_OPTIMALE_TROUVEE_MAIS_QUELQUES_CONTRAINTES_SONT_VIOLEES ) {
		 
  fprintf( FlotDeSortie , "VARIABLES VALUES;\n" );
  for ( i = 0 ; i < Mps.NbVar ; i++ ) {
    fprintf( FlotDeSortie , "%s;%e\n" , Mps.LabelDeLaVariable[i], Mps.U[i] ); 
	}
  fprintf( FlotDeSortie , "DUAL VARIABLES;\n" ); 
  for ( Cnt = 0 ; Cnt < Mps.NbCnt ; Cnt++ ) {
    fprintf( FlotDeSortie , "%s;%e\n" ,Mps.LabelDeLaContrainte[Cnt], Mps.VariablesDualesDesContraintes[Cnt] );
	}
	fclose( FlotDeSortie );
	
  Critere = 0.;
  for ( i = 0 ; i < Mps.NbVar ; i++ ) {
    Critere+= Mps.CoefsObjectif[i] * Mps.U[i];
    if ( AfficherLesValeursDesVariables == OUI_PNE ) {
      printf("Variable number %d name %s value %lf ",i, Mps.LabelDeLaVariable[i], Mps.U[i]);
      if ( Mps.TypeDeBorneDeLaVariable[i] == VARIABLE_FIXE ) printf(" FIXED variable\n");
      if ( Mps.TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_DES_DEUX_COTES ) {
        if ( Mps.TypeDeVariable[i] == ENTIER ) printf(" min %lf max %lf binary variable\n",Mps.Umin[i], Mps.Umax[i]);
        else printf(" min %lf max %lf\n",Mps.Umin[i], Mps.Umax[i]);
			}
      if ( Mps.TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_INFERIEUREMENT ) 
        printf(" min %lf max +INFINI\n",Mps.Umin[i]);
      if ( Mps.TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_SUPERIEUREMENT ) 
        printf(" min -INFINI max %lf\n",Mps.Umax[i]);
      if ( Mps.TypeDeBorneDeLaVariable[i] == VARIABLE_NON_BORNEE ) 
        printf(" min -INFINI max +INFINI\n");
    }
  }
  if ( AfficherLesValeursDesVariables == OUI_PNE ) {
    printf("\n********** Binary variables output **********\n\n");
    for ( i = 0 ; i < Mps.NbVar ; i++ ) {
      if ( Mps.TypeDeVariable[i] == ENTIER ) {
        printf("Variable number %d name %s value %lf\n",i, Mps.LabelDeLaVariable[i], Mps.U[i]);
      }
    }
  }
  if ( YaUneSolution == SOLUTION_OPTIMALE_TROUVEE ||
       YaUneSolution == SOLUTION_OPTIMALE_TROUVEE_MAIS_QUELQUES_CONTRAINTES_SONT_VIOLEES ) {								  
    printf("\n********** Optimal solution found, objective %e **********\n\n",Critere);
  }
  else if ( YaUneSolution == ARRET_PAR_LIMITE_DE_TEMPS_AVEC_SOLUTION_ADMISSIBLE_DISPONIBLE ) {
    printf("\n********** Feasible solution found, objective %e **********\n\n",Critere);
  }

  if ( VerifierLesContraintes == OUI_PNE ) {

  for ( i = 0 ; i < Mps.NbVar ; i++ ) {
    if ( Mps.TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_DES_DEUX_COTES ) {
      if ( Mps.U[i] < Mps.Umin[i] - 1.e-6 ) {
        printf("Variable number %d name %s violated lower bound: lo= %e value= %e violation= %e\n",
				        i, Mps.LabelDeLaVariable[i],Mps.Umin[i],Mps.U[i],Mps.Umin[i]-Mps.U[i]);
		  }
      else if ( Mps.U[i] > Mps.Umax[i] + 1.e-6 ) {
        printf("Variable number %d name %s violated upper bound: up= %e value= %e violation= %e\n",
				        i, Mps.LabelDeLaVariable[i],Mps.Umax[i],Mps.U[i],Mps.U[i]-Mps.Umax[i]);
			}
			continue;
		}
    if ( Mps.TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_INFERIEUREMENT ) {
      if ( Mps.U[i] < Mps.Umin[i] - 1.e-6 ) {
        printf("Variable number %d name %s violated lower bound: lo= %e value= %e violation= %e\n",
				        i, Mps.LabelDeLaVariable[i],Mps.Umin[i],Mps.U[i],Mps.Umin[i]-Mps.U[i]);
		  }
			continue;
		}
    if ( Mps.TypeDeBorneDeLaVariable[i] == VARIABLE_BORNEE_SUPERIEUREMENT ) {
      if ( Mps.U[i] > Mps.Umax[i] + 1.e-6 ) {
        printf("Variable number %d name %s violated upper bound: up= %e value= %e violation= %e\n",
				        i, Mps.LabelDeLaVariable[i],Mps.Umax[i],Mps.U[i],Mps.U[i]-Mps.Umax[i]);
			}
			continue;
		}    
  }	

  Smx   = -1.;
  EcX   = 1.e-6;
  CntMx = -1;
  EcMoy = 0.;
	
  for ( Cnt = 0 ; Cnt < Mps.NbCnt ; Cnt++ ) {	
    S = 0.;
    il    = Mps.Mdeb[Cnt];
    ilMax = il + Mps.NbTerm[Cnt];
    while ( il < ilMax ) {
      i = Mps.Nuvar[il];
      S+= Mps.A[il] * Mps.U[i];
      il++;
    }
    if ( Mps.SensDeLaContrainte[Cnt] == '=' ) {
      EcMoy+= fabs( S - Mps.Rhs[Cnt] );
      if ( fabs( S - Mps.Rhs[Cnt] ) > EcX ) {
        EcX   = fabs( S - Mps.Rhs[Cnt] );
        Smx   = S;
        CntMx = Cnt;
				printf("Cnt %d sens = : S %e B %e\n",Cnt,S,Mps.Rhs[Cnt]);
      }
    }
    if ( Mps.SensDeLaContrainte[Cnt] == '>' && S < Mps.Rhs[Cnt] ) {
      EcMoy+= fabs( S - Mps.Rhs[Cnt] );      
      if ( fabs( S - Mps.Rhs[Cnt] ) > EcX ) {
        EcX   = fabs( S - Mps.Rhs[Cnt] );
        Smx   = S;
        CntMx = Cnt;
				printf("Cnt %d sens > : S %e B %e\n",Cnt,S,Mps.Rhs[Cnt]);
      }
    }
    if ( Mps.SensDeLaContrainte[Cnt] == '<' && S > Mps.Rhs[Cnt] ) {
      EcMoy+= fabs( S - Mps.Rhs[Cnt] );      
      if ( fabs( S - Mps.Rhs[Cnt] ) > EcX ) {
        EcX   = fabs( S - Mps.Rhs[Cnt] );
        Smx   = S;
        CntMx = Cnt;
				printf("Cnt %d sens < : S %e B %e\n",Cnt,S,Mps.Rhs[Cnt]);
      }
    }
		/*printf("cnt %ld S %e B %e\n",Cnt,S,Mps.B[Cnt]);*/		
  }
  if ( CntMx >= 0 ) {
    printf("Higher violation:\n");
    if ( Mps.SensDeLaContrainte[CntMx] == '=' ) printf("Cnt num %d - %s -- type = ",CntMx,Mps.LabelDeLaContrainte[CntMx]);
    if ( Mps.SensDeLaContrainte[CntMx] == '<' ) printf("Cnt num %d - %s -- type < ",CntMx,Mps.LabelDeLaContrainte[CntMx]);
    if ( Mps.SensDeLaContrainte[CntMx] == '>' ) printf("Cnt num %d - %s -- type > ",CntMx,Mps.LabelDeLaContrainte[CntMx]);
    printf(" B %e computed %e violation %e\n",Mps.Rhs[CntMx],Smx,fabs(Smx-Mps.Rhs[CntMx]));
  }
  EcMoy/= Mps.NbCnt;
  printf("Violations average value: %e\n",EcMoy);

  }
   
}

if (AfficherLesValeursDesParametres == OUI_PNE) {
	printf("Valeurs des parametres :\n");
	printf(PNE_EcrireParams(pneParams, OUI_PNE));
	printf(SPX_EcrireParams(pneParams->spx_params, OUI_PNE));
}

if (nomFichierOutputParams != NULL) {
	PNE_EcrireJeuDeParametres_AvecNom(pneParams, nomFichierOutputParams, OUI_PNE);
}

printf("\n");
exit(0);
}
