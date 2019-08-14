
#include "rte_solver_interface.h"

namespace pne_library {
	#ifdef __cplusplus
		extern "C" {
	#endif
		#include <spx_definition_arguments.h>
		#include <spx_fonctions.h>
		#include <spx_define.h>	

		#include <pi_definition_arguments.h>
		#include <pi_constantes_externes.h>
		#include <pi_fonctions.h>

		#include <pne_definition_arguments.h>
		#include <pne_fonctions.h>

	#ifdef __cplusplus
		}
	#endif
}

pne_library::PROBLEME_A_RESOUDRE * convert(PROBLEME_A_RESOUDRE * Probleme) {
	pne_library::PROBLEME_A_RESOUDRE * pne_Probleme = new pne_library::PROBLEME_A_RESOUDRE;
	pne_Probleme->AffichageDesTraces = Probleme->AffichageDesTraces;

	return pne_Probleme;
}
void _PNE_Solveur(PROBLEME_A_RESOUDRE * Probleme, PNE_PARAMS * pneParams ) {
	pne_library::PROBLEME_A_RESOUDRE * pne_Probleme = convert(Probleme);

	//ERD FIXME verifier s'il faut un fichier de param ou un autre moyen de gerer ca
	pne_library::PNE_Solveur(pne_Probleme, pneParams);

	delete pne_Probleme;
}
void _PI_Quamin(PROBLEME_POINT_INTERIEUR * Probleme) {
	//PI_Quamin(Probleme);
}
void _SPX_LibererProbleme(PROBLEME_SPX * Spx) {
	//SPX_LibererProbleme(Spx);
}
void _SPX_ModifierLeVecteurCouts(PROBLEME_SPX * Spx, double * C_E, int NbVar_E) {
	//SPX_ModifierLeVecteurCouts(Spx, C_E, NbVar_E);
}
void _SPX_ModifierLeVecteurSecondMembre(PROBLEME_SPX * Spx, double * B_E, char * Sens_E, int NbCnt_E) {
	//SPX_ModifierLeVecteurSecondMembre(Spx, B_E, Sens_E, NbCnt_E);
}
PROBLEME_SPX * _SPX_Simplexe(PROBLEME_SIMPLEXE * Probleme, PROBLEME_SPX * Spx) {
	//return SPX_Simplexe( Probleme, Spx);
	return 0;
}