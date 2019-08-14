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
#ifndef PARAMS_PNE_DEJA_DEFINIES
#undef SEUIL_DADMISSIBILITE

#define PNE_PARAMS_LIST																	\
/* Parametres techniques du module */\
	MACRO_PRM(		VERBOSE_PNE,					int,		0						) PRM_SEP\
	MACRO_PRM(		DEBUG_PNE,						int,		0						) PRM_SEP\
	MACRO_PRM(		AffichageDesTraces,				int,		NON_PNE					) PRM_SEP\
	MACRO_PRM(		FaireDuPresolve,				int,		OUI_PNE					) PRM_SEP\
	/* Parametres d'optimisation */\
	MACRO_PRM(		TempsDExecutionMaximum,			int,		0						) PRM_SEP\
	MACRO_PRM(		NombreMaxDeSolutionsEntieres,	int,		1e6						) PRM_SEP\
	MACRO_PRM(		ToleranceDOptimalite,			double,		1e-12					) PRM_SEP\
	MACRO_PRM(		TOLERANCE_SUR_LES_ENTIERS,											double,		1.e-6	) PRM_SEP\
	/* Tolerance sur la satifaction des contraintes */\
	MACRO_PRM(		SEUIL_DADMISSIBILITE,												double,		1.e-6	)

#define PNE_PARAMS_LIST_FIXED																	\
	/************************************************************************/\
	MACRO_PRM(		MPCC_DANS_PI,														int,		NON_PNE	) PRM_SEP\
	MACRO_PRM(		NB_ECHECS_INHIB_MIR,												int,		3		) PRM_SEP\
	MACRO_PRM(		NB_TERMES_FORCE_CALCUL_DE_K,										int,		200		) PRM_SEP\
	MACRO_PRM(		SEUIL_VARIABLE_DUALE_POUR_CALCUL_DE_COUPE,							double,		1.e-6	) PRM_SEP\
	/* Constantes pour les coupes de Gomory.\
	   Les valeurs par defaut s'appuient sur une etude de Cornuejols Margot et Nannicini: On the Safety of Gomory cut generators */\
	MACRO_PRM(		NORMALISER_LES_COUPES_SUR_LES_G_ET_I,								int,		NON_PNE	) PRM_SEP\
	MACRO_PRM(		SEUIL_POUR_NORMALISER_LES_COUPES_SUR_LES_G_ET_I,					double,		1.e-6	) PRM_SEP\
	/************************************************************************/\
	/* Valeurs utilisees dans le simplexe */\
	MACRO_PRM(		RAPPORT_MAX_COEFF_COUPE_GOMORY,										double,		1.e+6	) PRM_SEP\
	MACRO_PRM(		RAPPORT_MAX_COEFF_COUPE_INTERSECTION,								double,		1.e+6	) PRM_SEP\
	MACRO_PRM(		ZERO_POUR_COEFF_VARIABLE_DECART_DANS_COUPE_GOMORY_OU_INTERSECTION,	double,		1.e-13	) PRM_SEP\
	MACRO_PRM(		ZERO_POUR_COEFF_VARIABLE_NATIVE_DANS_COUPE_GOMORY_OU_INTERSECTION,	double,		1.e-11	) PRM_SEP\
	MACRO_PRM(		RELAX_RHS_GOMORY_ABS,												double,		0.e-9	) PRM_SEP\
	MACRO_PRM(		RELAX_RHS_GOMORY_REL,												double,		0.e-10	) PRM_SEP\
	MACRO_PRM(		RELAX_RHS_INTERSECTION_ABS,											double,		1.e-7	) PRM_SEP\
	MACRO_PRM(		RELAX_RHS_INTERSECTION_REL,											double,		1.e-8	) PRM_SEP\
	MACRO_PRM(		SEUIL_FRACTIONNALITE_POUR_FAIRE_UNE_COUPE_DE_GOMORY,				double,		1.e-4	) PRM_SEP\
	MACRO_PRM(		SEUIL_FRACTIONNALITE_POUR_COUPE_INTERSECTION,						double,		1.e-4	) PRM_SEP\
	MACRO_PRM(		CALCULER_COUPES_DE_GOMORY,										int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		CALCULER_COUPES_DINTERSECTION,									int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		CALCULER_COUPES_KNAPSACK_SIMPLE,									int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		CALCULER_MIR_MARCHAND_WOLSEY,										int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		UTILISER_LE_GRAPHE_DE_CONFLITS,									int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		FAIRE_DU_NODE_PRESOLVE,											int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		REDUCED_COST_FIXING_AU_NOEUD_RACINE,								int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		REDUCED_COST_FIXING_AUX_NOEUD_DANS_ARBRE,							int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		RELANCE_PERIODIQUE_DU_SIMPLEXE_AU_NOEUD_RACINE,					int,		NON_PNE	) PRM_SEP\
	MACRO_PRM(		UTILISER_LES_COUPES_DE_PROBING,									int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		CONSTRUIRE_BORNES_VARIABLES,										int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		UTILISER_UNE_CONTRAINTE_DE_COUT_MAX,								int,		NON_PNE	) PRM_SEP\
	/* Pour eviter de recalculer le cout de la solution dans le simplexe et la comparer\
	au cout de la meilleure solution pour eventuellement arreter les iteration */\
	MACRO_PRM(		SEUIL_VIOLATION_COUPE_DE_GOMORY,									double,		1.e-6	) PRM_SEP\
	MACRO_PRM(		SEUIL_VIOLATION_COUPE_DINTERSECTION,								double,		1.e-5	) PRM_SEP\
	MACRO_PRM(		SEUIL_VIOLATION_KNAPSACK,											double,		1.e-3	) PRM_SEP\
	MACRO_PRM(		SEUIL_VIOLATION_MIR_MARCHAND_WOLSEY,								double,		1.e-2	) PRM_SEP\
	MACRO_PRM(		SEUIL_VIOLATION_CLIQUES,											double,		1.e-3	) PRM_SEP\
	MACRO_PRM(		SEUIL_VIOLATION_IMPLICATIONS,										double,		1.e-3	) PRM_SEP\
	/* On met un seuil eleve car il y a generalement beaucoup de coupes de probing */\
	MACRO_PRM(		SEUIL_VIOLATION_COUPES_DE_PROBING,									double,		1.e-1	) PRM_SEP\
	MACRO_PRM(		SEUIL_VIOLATION_BORNES_VARIABLES,									double,		1.e-2	) PRM_SEP\
	/************************************************************************/\
	/* Parametres de GUB */\
	MACRO_PRM(		UTILISER_LES_GUB,													int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		MIN_TERMES_GUB,														int,		4	) PRM_SEP\
	MACRO_PRM(		MAX_TERMES_GUB,														int,		10	) PRM_SEP\
	MACRO_PRM(		PROFONDEUR_LIMITE_POUR_UTILISATION_DES_GUB,							int,		100000	) PRM_SEP\
	/************************************************************************/\
	/* Parametres de Knapsack */\
	MACRO_PRM(		VIOLATION_MIN_POUR_K_SUR_COUPE,										double,		1.e-2	) PRM_SEP\
	/* Experimentalement le K sur Gomory sont interesantes mais dans certains cas cela pose des problemes numeriques: en particulier c'est incompatible avec \
	l'option UTILISER_UNE_CONTRAINTE_DE_COUT_MAX car cette contrainte peut etre tres pleine si beaucoup de variables ont un cout non nul */\
	MACRO_PRM(		KNAPSACK_SUR_GOMORY,												int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		KNAPSACK_SUR_COUPE_DINTERSECTION,									int,		NON_PNE	) PRM_SEP\
	MACRO_PRM(		KNAPSACK_SUR_COMBINAISONS_DE_CONTRAINTES,							int,		NON_PNE	) PRM_SEP\
	MACRO_PRM(		CALCULS_SUR_MIXED_0_1_KNAPSACK,									int,		NON_PNE	) PRM_SEP\
	MACRO_PRM(		KNAPSACK_SUR_CONTRAINTES_DEGALITE,								int,		NON_PNE	) PRM_SEP\
	MACRO_PRM(		MIN_TERMES_POUR_KNAPSACK,											int,		3	) PRM_SEP\
	/* Il faut limiter car si la contrainte a trop de termes la recherche des K est trop longue */\
	MACRO_PRM(		MAX_TERMES_POUR_KNAPSACK,											int,		10000	) PRM_SEP\
	/* Non utilise */\
	MACRO_PRM(		MAX_TERMES_POUR_KNAPSACK_COMBINEES,									int,		10000	) PRM_SEP\
	MACRO_PRM(		SEUIL_POUR_PROGRAMMATION_DYNAMIQUE,									int,		256	) PRM_SEP\
	/* Malgre beaucoup de test on n'a pas vu de cas ou la programmation dynamique faisait mieux */\
	MACRO_PRM(		UTILISER_AUSSI_LA_PROGRAMMATION_DYNAMIQUE,						int,		NON_PNE	) PRM_SEP\
	MACRO_PRM(		COEFFS_ENTIERS_DANS_KNAPSACK,										int,		NON_PNE	) PRM_SEP\
	/************************************************************************/\
	MACRO_PRM(		PNE_ACTIVATION_SUPPRESSION_PETITS_TERMES,							int,		NON_PNE	) PRM_SEP\
	/************************************************************************/\
	/* Pour les heuristiques */\
	MACRO_PRM(		CYCLE_HEURISTIQUES,													int,		3	) PRM_SEP\
	MACRO_PRM(		NB_MAX_ECHECS_SUCCESSIFS_HEURISTIQUE,								int,		3	) PRM_SEP\
	MACRO_PRM(		NB_MAX_REFUS_SUCCESSIFS,											int,		2	) PRM_SEP\
	/* Quand on depasse ce seuil sans trouver de solution on arrete l'heuristique */\
	MACRO_PRM(		NB_MAX_REACTIVATION_SANS_SUCCES,									int,		3	) PRM_SEP\
	MACRO_PRM(		UTILISER_LES_COUPES,												int,		NON_PNE	) PRM_SEP\
	/************************************************************************/\
	/* Pour la reduction des coeff des variables entieres */\
	MACRO_PRM(		EPS_COEFF_REDUCTION,												double,		1.e-3	) PRM_SEP\
	MACRO_PRM(		DELTA_MIN_REDUCTION,												double,		1.e-1	) PRM_SEP\
	MACRO_PRM(		MARGE_REDUCTION,													double,		1.e-3	) PRM_SEP\
	MACRO_PRM(		NB_ITER_MX_REDUCTION,												int,		5	) PRM_SEP\
	/************************************************************************/\
	/* Pour le node presolve et le probing */\
	MACRO_PRM(		PROBING_JUSTE_APRES_LE_PRESOLVE,									int,		OUI_PNE	) PRM_SEP\
	MACRO_PRM(		PROFONDEUR_MIN_POUR_NODE_PRESOLVE,									int,		2	) PRM_SEP\
	MACRO_PRM(		CYCLE_NODE_PRESOLVE,												int,		1	) PRM_SEP\
	MACRO_PRM(		MARGE_INITIALE,														double,		1.e-6	) PRM_SEP\
	MACRO_PRM(		ZERO_NP_PROB,														double,		1.e-8	) PRM_SEP\
	/************************************************************************/\
	/* Nombre de termes qu'on peut ajouter a chaque contrainte */\
	MACRO_PRM(		MARGE_EN_FIN_DE_CONTRAINTE,											int,		5	) PRM_SEP\
/* Parametres a priori non modifies par l'utilisateur */\
	MACRO_PRM(		VALEUR_DE_FRACTIONNALITE_NULLE,									double,		1.e-8	) PRM_SEP\
	MACRO_PRM(		INCREMENT_DALLOCATION_POUR_LE_NOMBRE_DE_VARIABLES_PNE,			int,		256		) PRM_SEP\
	MACRO_PRM(		INCREMENT_DALLOCATION_POUR_LE_NOMBRE_DE_CONTRAINTES_PNE,		int,		256		) PRM_SEP\
	MACRO_PRM(		INCREMENT_DALLOCATION_POUR_LA_MATRICE_DES_CONTRAINTES_PNE,		int,		256		) PRM_SEP\
	MACRO_PRM(		PLAGE_REDUCED_COST_FIXING,										double,		1.e+15	)

struct SPX_PARAMS_S;
typedef struct SPX_PARAMS_S SPX_PARAMS;
struct PNE_PARAMS_S {
	#define MACRO_PRM(prm_name, prm_type, prm_default) prm_type prm_name
	#define PRM_SEP ;
	PNE_PARAMS_LIST;
	PNE_PARAMS_LIST_FIXED;
	#undef PRM_SEP
	#undef MACRO_PRM
	SPX_PARAMS * spx_params;
};

typedef struct PNE_PARAMS_S PNE_PARAMS;

PNE_PARAMS* newDefaultPneParams();

void PNE_LireJeuDeParametres_AvecNom(PNE_PARAMS* Params, char const * file_name, int verbose);
void PNE_EcrireJeuDeParametres_AvecNom(PNE_PARAMS* Params, char const * file_name, int DiffSeules);
char* PNE_EcrireParams(PNE_PARAMS *Params, int DiffSeules);

void PNE_setIntParam(PNE_PARAMS* Params, char const * param_name, int value);
void PNE_setDoubleParam(PNE_PARAMS* Params, char const * param_name, double value);
void PNE_setCharParam(PNE_PARAMS* Params, char const * param_name, char value);

# define PARAMS_PNE_DEJA_DEFINIES
# endif  
# ifdef __cplusplus
  }
# endif		
