// Copyright (C) 2007-2022, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// SPDX-License-Identifier: Apache-2.0

/*************************************************************************

   FONCTION: Recherche du noeud avec le plus grand ratio :
             Nombre de variables a valeur entiere / profondeur du noeud

   AUTEUR: R. GONZALEZ

**************************************************************************/

#include "bb_sys.h"
#include "bb_define.h"
#include "bb_fonctions.h"

# ifdef BB_UTILISER_LES_OUTILS_DE_GESTION_MEMOIRE_PROPRIETAIRE	
  # include "bb_memoire.h"
# endif

/*---------------------------------------------------------------------------------------------------------*/
/* TypeDeRecherche pour choisir le noeud le plus profond ou le noeud avec le moins de variables
fractionnaires */

NOEUD * BB_RechercherLeNoeudLeAvecMeilleurRatioFractionnaire( BB * Bb )
{
NOEUD ** NoeudsAExplorer; NOEUD ** NoeudsAExplorerAuProchainEtage;  NOEUD *  Noeud;
NOEUD * NoeudCourant;  int NombreDeNoeudsAExplorer; int NombreDeNoeudsAExplorerAuProchainEtage;
int i; NOEUD * NoeudChoisi; char OnPrend; double X; double MeilleurRatio;

NoeudChoisi = 0;
MeilleurRatio = 1.e+80;

NoeudsAExplorer = (NOEUD **) malloc( 1 * sizeof( void * ) );
if ( NoeudsAExplorer == NULL ) { 
  printf("\n Saturation memoire dans la partie branch & bound, fonction:  BB_RechercherUnNouveauNoeudDeDepart \n");
  Bb->AnomalieDetectee = OUI;
  longjmp( Bb->EnvBB , Bb->AnomalieDetectee ); 
}
NombreDeNoeudsAExplorer = 1;
NoeudsAExplorer[0]      = Bb->NoeudRacine;

while( 1 ) {
  NoeudsAExplorerAuProchainEtage = (NOEUD **) malloc( NombreDeNoeudsAExplorer * 2 * sizeof( void * ) );
  if ( NoeudsAExplorerAuProchainEtage == NULL ) { 
    printf("\n Saturation memoire dans la partie branch & bound, fonction: BB_EliminerLesNoeudsSousOptimaux 2 \n");
    Bb->AnomalieDetectee = OUI;
    longjmp( Bb->EnvBB , Bb->AnomalieDetectee ); 
  }
  NombreDeNoeudsAExplorerAuProchainEtage = 0;
  for( i = 0 ; i < NombreDeNoeudsAExplorer ; i++ ) {
    NoeudCourant = NoeudsAExplorer[i];
    if ( NoeudCourant != 0 ) {
      OnPrend = NON;
      if ( NoeudCourant == Bb->NoeudRacine ) goto Next;
      X = (double) (Bb->NombreDeVariablesEntieresDuProbleme - NoeudCourant->NbValeursFractionnairesApresResolution);
      X/= (double) NoeudCourant->ProfondeurDuNoeud;   
      if ( NoeudCourant->StatutDuNoeud == EVALUE && NoeudCourant->NoeudTerminal != OUI ) {
        /* Doit avoir au moins 1 fils a evaluer */	
        Noeud = NoeudCourant->NoeudSuivantGauche;
        if ( Noeud != 0 ) {
	  if ( Noeud->StatutDuNoeud == A_EVALUER ) {	  
            if ( NoeudChoisi == 0 )       { OnPrend = OUI; goto FinTest; }
	    else if ( X > MeilleurRatio ) { OnPrend = OUI; goto FinTest; }
	  }
	}	
        Noeud = NoeudCourant->NoeudSuivantDroit;
        if ( Noeud != 0 ) {
	  if ( Noeud->StatutDuNoeud == A_EVALUER ) {
            if ( NoeudChoisi == 0 )       { OnPrend = OUI; goto FinTest; }
	    else if ( X > MeilleurRatio ) { OnPrend = OUI; goto FinTest; }	  	   	 	    
	  }
	}
	FinTest:
	if ( OnPrend == OUI ) {
	  NoeudChoisi   = NoeudCourant;
	  MeilleurRatio = X;
	}
      }     
      /* Renseigner la table des noeuds du prochain etage */
      Next:
      Noeud = NoeudCourant->NoeudSuivantGauche;
      if ( Noeud != 0 ) {
        NombreDeNoeudsAExplorerAuProchainEtage++;
        NoeudsAExplorerAuProchainEtage[NombreDeNoeudsAExplorerAuProchainEtage-1] = Noeud;
      }
      Noeud = NoeudCourant->NoeudSuivantDroit;
      if ( Noeud != 0 ) {	    
        NombreDeNoeudsAExplorerAuProchainEtage++;
        NoeudsAExplorerAuProchainEtage[NombreDeNoeudsAExplorerAuProchainEtage-1] = Noeud;          
      }
    }
  } 
                        /* Preparations pour l'etage suivant */
  if( NombreDeNoeudsAExplorerAuProchainEtage == 0 ) break; /* Exploration de l'arbre terminee */
  free( NoeudsAExplorer );
  NoeudsAExplorer = (NOEUD **) malloc( NombreDeNoeudsAExplorerAuProchainEtage * sizeof( void * ) );
  if ( NoeudsAExplorer == NULL ) { 
    printf("\n Saturation memoire dans la partie branch & bound, fonction: BB_EliminerLesNoeudsSousOptimaux 3 \n");
    Bb->AnomalieDetectee = OUI;
    longjmp( Bb->EnvBB , Bb->AnomalieDetectee ); 
  }
  NombreDeNoeudsAExplorer = NombreDeNoeudsAExplorerAuProchainEtage; 
  for( i = 0 ; i < NombreDeNoeudsAExplorer ; i++ ) {
    NoeudsAExplorer[i] = NoeudsAExplorerAuProchainEtage[i];
  }
  free( NoeudsAExplorerAuProchainEtage );
/* fin while */
}	    

free( NoeudsAExplorer );
free( NoeudsAExplorerAuProchainEtage );

return( NoeudChoisi );

}







