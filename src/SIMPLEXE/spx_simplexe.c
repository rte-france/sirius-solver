// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

/***********************************************************************

   FONCTION: Resolution de Min c x sous contrainte Ax = b par un  
             simplexe (forme revisee du simplexe) en matrices  
             creuses : appel de la routine principale.
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_fonctions.h"
# include "spx_define.h"

# ifdef SPX_UTILISER_LES_OUTILS_DE_GESTION_MEMOIRE_PROPRIETAIRE	
  # include "spx_memoire.h"
# endif

void SPX_InitSetJmp( jmp_buf );

/*---------------------------------------------------------------------------------------------------------*/

void SPX_InitSetJmp( jmp_buf BufferDeStockage )
{
setjmp( BufferDeStockage ); 
return;
}

/*---------------------------------------------------------------------------------------------------------*/

PROBLEME_SPX * SPX_Simplexe( PROBLEME_SIMPLEXE * Probleme , PROBLEME_SPX * Spx , SPX_PARAMS * spx_params ) 
{
void * Tas;

if ( Spx == NULL ) {
  # ifdef SPX_UTILISER_LES_OUTILS_DE_GESTION_MEMOIRE_PROPRIETAIRE	
    Tas = MEM_Init(); 
    Spx = (PROBLEME_SPX *) MEM_Malloc( Tas, sizeof( PROBLEME_SPX ) );
    if ( Spx == NULL ) {
      printf("Saturation memoire, impossible d'allouer un objet PROBLEME_SPX\n");    
      Probleme->ExistenceDUneSolution = SPX_ERREUR_INTERNE;
      return( Spx );
    }
		memset( (char *) Spx, 0, sizeof( PROBLEME_SPX ) );
	  Spx->Tas = Tas;
  # else
	  Tas = NULL;
    Spx = (PROBLEME_SPX *) malloc( sizeof( PROBLEME_SPX ) );
    if ( Spx == NULL ) {
      printf("Saturation memoire, impossible d'allouer un objet PROBLEME_SPX\n");    
      Probleme->ExistenceDUneSolution = SPX_ERREUR_INTERNE;
      return( Spx );
    }
		memset( (char *) Spx, 0, sizeof( PROBLEME_SPX ) );
	  Spx->Tas = Tas;
	# endif
}

if ( Spx->spx_params == NULL )
{
	if ( spx_params != NULL )
	{
		Spx->spx_params = spx_params;
	}
	else
	{
		Spx->spx_params = newDefaultSpxParams();
	}
}

Spx->AnomalieDetectee = NON_SPX;

setjmp( Spx->EnvSpx );

/* Pour ne pas avoir de warning a la compilation */
/* Attention, il ne faut pas faire appel à a une autre routine pour faire le setjmp
   car lorsque le longjmp arrive, au return de la routine en question on se retrouve
	 n'importe ou et ça plante */
/*SPX_InitSetJmp( Spx->EnvSpx );*/

if ( Spx->AnomalieDetectee != NON_SPX ) {
  /* Liberation du probleme */ 
   /* Meme si une anomalie a ete detectee il est preferable de ne pas liberer le probleme 
      ici. Le probleme est de toute facon libere en fin de PNE . */ 
   /* SPX_LibererProbleme( Spx ); */     
  Probleme->ExistenceDUneSolution = SPX_ERREUR_INTERNE;
  if ( Spx->AnomalieDetectee == SPX_MATRICE_DE_BASE_SINGULIERE ) {
    Probleme->ExistenceDUneSolution = SPX_MATRICE_DE_BASE_SINGULIERE;
    /*printf("Trace simplexe: Matrice de base singuliere\n");*/
  }
  return( Spx );     
} 
else {
  /* Optimisation */
 SPX_SimplexeCalculs( Probleme , Spx );
 /* On ne renvoie pas de pointeur a la structure si sa desallocation 
    a ete demandee par l'appelant */
 if ( Probleme->LibererMemoireALaFin == OUI_SPX ) Spx = NULL;
}
 
return( Spx );
}  











