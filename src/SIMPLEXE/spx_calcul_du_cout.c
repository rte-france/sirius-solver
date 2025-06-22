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

   FONCTION: Utilisable surtout dans un contexte de Branch and Bound.
             On calcule le cout de la solution primale et on le compare 
             au seuil fourni en entr�e (qui dans un contexte de 
             Branch and Bound est le cout de la meilleure solution 
             entiere deja trouvee). Si le cout calcul� est superieur
             au seuil fourni en entree, on arrete les calculs et on 
             sort avec le verdict: pas de solution. En effet le cout
             courant est un minorant du cout optimal.
                
   AUTEUR: R. GONZALEZ

************************************************************************/

# include "spx_sys.h"

# include "spx_define.h"
# include "spx_fonctions.h"

# include "pne_define.h"

# define TRACES 0

/*----------------------------------------------------------------------------*/

void SPX_CalculDuCout( PROBLEME_SPX * Spx )
{
int i; double Cout; double * C; double * X; PROBLEME_PNE * Pne; 
# if TRACES == 1
  double * Csv; double VraiCout; double Erreur;
# endif

SPX_FixerXEnFonctionDeSaPosition( Spx );
char msg[SIRIUS_CALLBACK_BUFFER_SIZE];

C = Spx->C;

# if TRACES == 1 
  Csv = Spx->Csv;
# endif

X = Spx->X;
Cout = 0.0;

# if TRACES == 1 
  VraiCout = 0;
# endif

for ( i = 0 ; i < Spx->NombreDeVariables ; i++ ) {	 
  Cout += C[i] * X[i];
  # if TRACES == 1 	
	  VraiCout += Csv[i] * X[i];
  # endif	
} 

Cout /= Spx->ScaleLigneDesCouts;
Cout += Spx->PartieFixeDuCout;

callback_function call_back = SPXgetcbmessage(Spx);

# if TRACES == 1 	
  VraiCout /= Spx->ScaleLigneDesCouts;
  VraiCout += Spx->PartieFixeDuCout;
	Erreur = fabs( VraiCout - Cout );
	snprintf(msg, SIRIUS_CALLBACK_BUFFER_SIZE, "Erreur due au bruitage des couts %e  Cout sans bruitage %e\n",Erreur,VraiCout);
  call_back(Spx->something_from_the_caller, msg, 0, SIRIUS_ERROR);
# endif	

Spx->Cout = Cout;

Pne = (PROBLEME_PNE *) Spx->ProblemePneDeSpx;
if ( Pne != NULL ) {
  Spx->Cout += Pne->Z0;	
}

#if VERBOSE_SPX
  if ( Spx->StrongBranchingEnCours != OUI_SPX ) {  
    if ( Spx->UtiliserCoutMax == OUI_SPX ) { 
      snprintf(msg, SIRIUS_CALLBACK_BUFFER_SIZE, "Iteration %5d Cout %20.6lf Infaisabilites primales %20.6lf PartieFixeDuCout %20.6lf CoutMax %20.6lf\n",
              Spx->Iteration,Spx->Cout,Spx->SommeDesInfaisabilitesPrimales,Spx->PartieFixeDuCout,Spx->CoutMax);
      call_back(Spx->something_from_the_caller, msg, 0, SIRIUS_INFO);
    }
    else {
      snprintf(msg, SIRIUS_CALLBACK_BUFFER_SIZE, "Iteration %5d Cout %20.6lf Infaisabilites primales %20.6lf PartieFixeDuCout %20.6lf\n",
              Spx->Iteration,Spx->Cout,Spx->SommeDesInfaisabilitesPrimales,Spx->PartieFixeDuCout);
      call_back(Spx->something_from_the_caller, msg, 0, SIRIUS_INFO);
    }
  }
#else
  /* Cas non verbose */		 
  if ( Spx->LaBaseDeDepartEstFournie == NON_SPX && Spx->AffichageDesTraces == OUI_SPX ) { /* Premier simplexe */
    if ( Spx->EcrireLegendePhase2 == OUI_SPX ) {
      Spx->EcrireLegendePhase1 = OUI_SPX;   
      Spx->EcrireLegendePhase2 = NON_SPX;   
      const char* info_msg = "  | Phase | Iteration |         Objective        |          Primal infeas.       |     Primal infeas. count   |\n";               
      call_back(Spx->something_from_the_caller, info_msg, 0, SIRIUS_INFO);

    }          
    snprintf(msg, SIRIUS_CALLBACK_BUFFER_SIZE, "  |   II  |   %6d  |      %16.9e    |        %15.8e        |         %10d         |\n",
     Spx->Iteration, Spx->Cout, Spx->SommeDesInfaisabilitesPrimales, Spx->NombreDeContraintesASurveiller);
  }
#endif

return;
}

