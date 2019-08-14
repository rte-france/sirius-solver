// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

# ifdef __cplusplus
extern "C"
{
# endif
#ifndef PARAMS_SPX_DEJA_DEFINIES
#undef SEUIL_DADMISSIBILITE
#define SPX_PARAMS_LIST																	\
/* Parametres techniques du module */\
	MACRO_PRM(		VERBOSE_SPX,									int,		0		) PRM_SEP\
	MACRO_PRM(		VERBOSE_SPX_SCALING,							int,		0		) PRM_SEP\
	MACRO_PRM(		SEUIL_DE_VIOLATION_DE_BORNE,					double,		1.e-7	) PRM_SEP\
	MACRO_PRM(		SEUIL_DADMISSIBILITE,							double,		1.e-6	) PRM_SEP\
	MACRO_PRM(		NOMBRE_MAX_DITERATIONS,							int,		100000	)

#define SPX_PARAMS_LIST_FIXED																	\
	MACRO_PRM(		COEFFICIENT_A_BASE_INITIALE_PHASE_1,			double,		1.		) PRM_SEP\
	MACRO_PRM(		SEUIL_MIN_DE_VIOLATION_DE_BORNE,				double,		0.01*1.e-7	) PRM_SEP\
	MACRO_PRM(		SEUIL_MAX_DE_VIOLATION_DE_BORNE,				double,		1000*1.e-7	) PRM_SEP\
	MACRO_PRM(		SEUIL_DE_VIOLATION_DE_BORNE_NON_NATIVE,			double,		1.e-7		) PRM_SEP\
	MACRO_PRM(		SEUIL_MIN_DE_VIOLATION_DE_BORNE_NON_NATIVE,		double,		0.01*1.e-7	) PRM_SEP\
	MACRO_PRM(		SEUIL_MAX_DE_VIOLATION_DE_BORNE_NON_NATIVE,		double,		1000*1.e-7	) PRM_SEP\
	MACRO_PRM(		SEUIL_DE_VIOLATION_DE_BORNE_VARIABLES_ECART_COUPES,	double,	1.e-7	) PRM_SEP\
	MACRO_PRM(		LINFINI_POUR_LE_COUT,							double,		1.e+80		) PRM_SEP\
	MACRO_PRM(		LINFINI_POUR_X,									double,		1.e+80		) PRM_SEP\
	MACRO_PRM(		SEUIL_DE_DEGENERESCENCE,						double,		1.e-7		) PRM_SEP\
	MACRO_PRM(		VALEUR_DE_PIVOT_ACCEPTABLE,						double,		1.e-7		) PRM_SEP\
	MACRO_PRM(		COEFF_AUGMENTATION_VALEUR_DE_PIVOT_ACCEPTABLE,	double,		10.0		) PRM_SEP\
	MACRO_PRM(		DIVISEUR_VALEUR_DE_PIVOT_ACCEPTABLE,			double,		1.1		) PRM_SEP\
	MACRO_PRM(		CYCLE_DE_REFACTORISATION_DUAL,					int,		50		) PRM_SEP\
	MACRO_PRM(		CYCLE_DE_REFACTORISATION,						int,		50		) PRM_SEP\
	MACRO_PRM(		SEUIL_DE_DEGENERESCENCE_DUAL,					double,		1.e-9		) PRM_SEP\
	MACRO_PRM(		NOMBRE_MAX_DE_CONTROLES_FINAUX,					int,		10		) PRM_SEP\
	MACRO_PRM(		SEUIL_POUR_RECONSTRUCTION_BASE,					int,		9		) PRM_SEP\
	/*MACRO_PRM(		SEUIL_ADMISSIBILITE_DUALE_1,					double,		1.e-8		) PRM_SEP*/\
	MACRO_PRM(		SEUIL_ADMISSIBILITE_DUALE_2,					double,		2.e-8		) PRM_SEP\
	MACRO_PRM(		COEFF_MIN_SEUIL_DUAL,							double,		0.01		) PRM_SEP\
	MACRO_PRM(		COEFF_MAX_SEUIL_DUAL,							double,		1000.		) PRM_SEP\
	MACRO_PRM(		SEUIL_POUR_MODIFICATION_DE_COUT,				double,		10*SEUIL_ADMISSIBILITE_DUALE_1	) PRM_SEP\
	MACRO_PRM(		COEFF_SEUIL_POUR_MODIFICATION_DE_COUT,			double,		10.		) PRM_SEP\
	MACRO_PRM(		NOMBRE_MAX_DE_PERTURBATIONS_DE_COUT,			int,		1		) PRM_SEP\
	MACRO_PRM(		VALEUR_PERTURBATION_COUT_A_POSTERIORI,			double,		100*defaultPneParams->SEUIL_POUR_MODIFICATION_DE_COUT	) PRM_SEP\
	MACRO_PRM(		COEFF_VALEUR_PERTURBATION_COUT_A_POSTERIORI,	double,		100.			) PRM_SEP\
	MACRO_PRM(		COEFF_TOLERANCE_POUR_LE_TEST_DE_HARRIS,			double,		0.1				) PRM_SEP\
	MACRO_PRM(		CYCLE_DE_VERIF_ADMISSIBILITE_DUALE,				int,		150			) PRM_SEP\
	MACRO_PRM(		MAX_BOUND_FLIP,									int,		1000		) PRM_SEP\
	MACRO_PRM(		NOMBRE_DITERATIONS_DE_STRONG_BRANCHING,			int,		5			) PRM_SEP\
	MACRO_PRM(		CYCLE_DE_CONTROLE_DE_DEGENERESCENCE_AGRESSIF,	int,		0		) PRM_SEP\
	MACRO_PRM(		CYCLE_DE_CONTROLE_DE_DEGENERESCENCE_PEU_AGRESSIF,	int,	5		) PRM_SEP\
/* Pour les coupes de Gomory */\
	MACRO_PRM(		SEUIL_DE_VERIFICATION_DE_NBarreR_GOMORY,		double,		1.e-7	) PRM_SEP\
	MACRO_PRM(		ZERO_GOMORY_1_F0,								double,		1.e-7	) PRM_SEP\
	MACRO_PRM(		ZERO_TERMES_DU_TABLEAU_POUR_GOMORY,				double,		1.e-14	) PRM_SEP\
/* Pour les bornes auxiliaires */\
	/*MACRO_PRM(		UTILISER_BORNES_AUXILIAIRES,					int,		1		) PRM_SEP*/\
	MACRO_PRM(		ITERATION_POUR_BORNES_AUXILIAIRES,				int,		0		) PRM_SEP\
	MACRO_PRM(		CYCLE_POUR_SUPPRESSION_DES_BORNES_AUXILIAIRES,	int,		100		) PRM_SEP\
/* Pour le creux et l'hyper creux */\
	MACRO_PRM(		TAUX_DE_REMPLISSAGE_POUR_BASE_HYPER_CREUSE,		double,		0.03		) PRM_SEP\
	MACRO_PRM(		TAUX_DE_REMPLISSAGE_POUR_VECTEUR_HYPER_CREUX,	double,		0.04		) PRM_SEP\
	MACRO_PRM(		SEUIL_ECHEC_CREUX,								int,		10		) PRM_SEP\
	MACRO_PRM(		SEUIL_ECHEC_CREUX_STEEPEST,						int,		20		) PRM_SEP\
	MACRO_PRM(		SEUIL_REUSSITE_CREUX,							int,		3		) PRM_SEP\
	MACRO_PRM(		CYCLE_TENTATIVE_HYPER_CREUX,					int,		4		) PRM_SEP\
	MACRO_PRM(		SEUIL_ABANDON_HYPER_CREUX,						int,		4		) PRM_SEP\
\
	MACRO_PRM(		SPX_ACTIVATION_SUPPRESSION_PETITS_TERMES,		int,		NON_SPX	) PRM_SEP\
	MACRO_PRM(		UTILISER_PNE_RAND,								int,		OUI_SPX	) PRM_SEP\
	/*MACRO_PRM(		VERIFICATION_PI        ,						int,		NON_SPX		) PRM_SEP\
	le code associe ne link pas car une fonction a disparu */\
	MACRO_PRM(		VERIFICATION_BBARRE    ,						int,		NON_SPX		) PRM_SEP\
	/*MACRO_PRM(		VERIFICATION_ERBMOINS1 ,						int,		NON_SPX		) PRM_SEP\
	le code associe ne link pas car une fonction a disparu */\
	/*MACRO_PRM(		VERIFICATION_ABARRES   ,						int,		NON_SPX		) PRM_SEP\
	le code associe ne compile pas*/\
	/*MACRO_PRM(		VERIFICATION_STEEPEST  ,						int,		NON_SPX		) PRM_SEP\
	le code associe ne compile pas*/\
	/*MACRO_PRM(		VERIFICATION_MAJ_BBARRE,						int,		NON_SPX		) PRM_SEP\
	le code associe ne compile pas*/\
	MACRO_PRM(		NB_MAX_DE_REACTIVATIONS_DE_LA_BASE_REDUITE,		int,		3		) PRM_SEP\
	MACRO_PRM(		NB_DE_BASE_REDUITE_SUCCESSIVES_SANS_PRISE_EN_COMPTE_DES__VIOLATIONS,	int,	20	) PRM_SEP\
	MACRO_PRM(		NB_DE_BASES_COMPLETES_SUCCESSIVES,				int,		5		) PRM_SEP\
\
	MACRO_PRM(		POIDS_DANS_VALEUR_DE_VIOLATION,					int,		OUI_SPX		) PRM_SEP\
	MACRO_PRM(		PRICING_AVEC_VIOLATIONS_STRICTES,				int,		NON_SPX		) PRM_SEP\
/* interet du params vs define discutable*/\
	MACRO_PRM(		INCREMENT_DALLOCATION_POUR_LE_NOMBRE_DE_VARIABLES_SPX,		int,		256		) PRM_SEP\
	MACRO_PRM(		INCREMENT_DALLOCATION_POUR_LE_NOMBRE_DE_CONTRAINTES_SPX,	int,		256		) PRM_SEP\
	MACRO_PRM(		INCREMENT_DALLOCATION_POUR_LA_MATRICE_DES_CONTRAINTES_SPX,	int,		4096	) PRM_SEP\
\
	MACRO_PRM(		FAIRE_UN_BRUITAGE_INITIAL_DES_COUTS,			int,		OUI_SPX	)

struct SPX_PARAMS_S {
	#define MACRO_PRM(prm_name, prm_type, prm_default) prm_type prm_name
	#define PRM_SEP ;
	SPX_PARAMS_LIST;
	SPX_PARAMS_LIST_FIXED;
	#undef PRM_SEP
	#undef MACRO_PRM
};

typedef struct SPX_PARAMS_S SPX_PARAMS;

SPX_PARAMS* newDefaultSpxParams();

void SPX_LireJeuDeParametres_AvecNom(SPX_PARAMS* Params, char const * file_name, int verbose);
void SPX_EcrireJeuDeParametres_AvecNom(SPX_PARAMS* Params, char const * file_name, int DiffSeules);

# define PARAMS_SPX_DEJA_DEFINIES 
# endif
# ifdef __cplusplus
  }
# endif		
