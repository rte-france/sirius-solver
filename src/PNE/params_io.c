// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

#include "pne_sys.h"
#include "spx_params.h"
#include "pne_params.h"
#include "params_utils.h"

char* PNE_EcrireParams(PNE_PARAMS *Params, int DiffSeules)
{
	int LgMax; int MaxNbParams;
	char * Differences; char * LigneParam;
	PNE_PARAMS *DefaultParams = newDefaultPneParams();

	LgMax = 100;
	MaxNbParams = 50;
	LigneParam = (char *)malloc(LgMax * sizeof(char));
	Differences = (char *)malloc(LgMax * sizeof(char) * MaxNbParams);
	sprintf(Differences, "");

#define MACRO_PRM(prm_name, prm_type, prm_default)\
	/*On teste deux a deux les valeurs respectives de Params et DefaultParams*/\
	if (!DiffSeules || Params->prm_name != DefaultParams->prm_name) {\
		EcrireLigneParam_##prm_type(#prm_name, Params->prm_name, LigneParam);\
		strcat(Differences, LigneParam);\
	}
#define PRM_SEP  
	PNE_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM

	return Differences;
}

char* SPX_EcrireParams(SPX_PARAMS *Params, int DiffSeules)
{
	int LgMax; int MaxNbParams;
	char * Differences; char * LigneParam;
	SPX_PARAMS *DefaultParams = newDefaultSpxParams();

	LgMax = 100;
	MaxNbParams = 50;
	LigneParam = (char *)malloc(LgMax * sizeof(char));
	Differences = (char *)malloc(LgMax * sizeof(char) * MaxNbParams);
	sprintf(Differences, "");

#define MACRO_PRM(prm_name, prm_type, prm_default)\
	/*On teste deux a deux les valeurs respectives de Params et DefaultParams*/\
	if (!DiffSeules || Params->prm_name != DefaultParams->prm_name) {\
		EcrireLigneParam_##prm_type(#prm_name, Params->prm_name, LigneParam);\
		strcat(Differences, LigneParam);\
	}
#define PRM_SEP  
	SPX_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM

	return Differences;
}

void PNE_EcrireJeuDeParametres_AvecNom(PNE_PARAMS* Params, char const * file_name, int DiffSeules)
{
	FILE * Flot;

	/* Ouverture du fichier en ecriture */
	Flot = fopen(file_name, "w");
	if (Flot == NULL) {
		printf("Erreur - l'ouverture du fichier contenant le jeu de parametres a echoue\n");
		exit(1);
	}

	/* On ecrit la ligne d'en-tete */
	fprintf(Flot, "Champ;Valeur\n");

	/* On ecrit les valeurs de parametres qui ne correspondent pas a l'instance par defaut pour les param PNE */
	fprintf(Flot, PNE_EcrireParams(Params, DiffSeules));

	/* On ecrit les valeurs de parametres qui ne correspondent pas a l'instance par defaut */
	fprintf(Flot, SPX_EcrireParams(Params->spx_params, DiffSeules));

	/* On libere le flot */
	fclose(Flot);
}

void SPX_EcrireJeuDeParametres_AvecNom(SPX_PARAMS* Params, char const * file_name, int DiffSeules)
{
	FILE * Flot;

	/* Ouverture du fichier en ecriture */
	Flot = fopen(file_name, "w");
	if (Flot == NULL) {
		printf("Erreur - l'ouverture du fichier contenant le jeu de parametres a echoue\n");
		exit(1);
	}

	/* On ecrit la ligne d'en-tete */
	fprintf(Flot, "Champ;Valeur\n");

	/* On ecrit les valeurs de parametres qui ne correspondent pas a l'instance par defaut */
	fprintf(Flot, SPX_EcrireParams(Params, DiffSeules));

	/* On libere le flot */
	fclose(Flot);
}

void PNE_SetParamSimple(PNE_PARAMS *Params, char *EtiquetteChamp, char *Valeur)
{
#define MACRO_PRM(prm_name, prm_type, prm_default)\
	/*Test sur chaque etiquette de champ de PNE_PARAMS*/\
	if (!strcmp(EtiquetteChamp, #prm_name)) {\
		/*On extrait de la chaine "Valeur" une valeur du type prm_type*/\
		Params->prm_name = ExtraireValeur_##prm_type(Valeur);\
	}
#define PRM_SEP else 
	PNE_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM
	else
#define MACRO_PRM(prm_name, prm_type, prm_default)\
	/*Test sur chaque etiquette de champ de SPX_PARAMS*/\
	if (!strcmp(EtiquetteChamp, #prm_name)) {\
		/*On extrait de la chaine "Valeur" une valeur du type prm_type*/\
		Params->spx_params->prm_name = ExtraireValeur_##prm_type(Valeur);\
	}
#define PRM_SEP else 
		SPX_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM

	else printf("Warning - etiquette de champ non reconnue : %s\n", EtiquetteChamp);
};

void SPX_SetParamSimple(SPX_PARAMS *Params, char *EtiquetteChamp, char *Valeur)
{
#define MACRO_PRM(prm_name, prm_type, prm_default)\
	/*Test sur chaque etiquette de champ de SPX_PARAMS*/\
	if (!strcmp(EtiquetteChamp, #prm_name)) {\
		/*On extrait de la chaine "Valeur" une valeur du type prm_type*/\
		Params->prm_name = ExtraireValeur_##prm_type(Valeur);\
	}
#define PRM_SEP else 
	SPX_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM

	else printf("Warning - etiquette de champ non reconnue : %s\n", EtiquetteChamp);
};

void PNE_LireJeuDeParametres_AvecNom(PNE_PARAMS *Params, char const *file_name, int verbose)
{
	if (file_name != NULL) {
		printf("Lecture du fichier de parametres %s\n", file_name);
	} else {
		printf("Erreur - fichier de parametres mal renseigne\n");
	}
	FILE * Flot; int LgMaxChamp; int LgMaxVal; int LgMaxLigne;
	char * LigneLue; char * EtiquetteChamp; char * ValeurLue;
	int NumeroLigne;

	LgMaxChamp = 50;
	LgMaxVal = 30;
	LgMaxLigne = LgMaxChamp + LgMaxVal + 1;

	/* Ouverture du fichier en lecture */
	Flot = fopen(file_name, "r");
	if (Flot == NULL) {
		printf("Erreur - l'ouverture du fichier contenant le jeu de parametres a echoue\n");
		exit(1);
	}

	LigneLue = (char *)malloc(LgMaxLigne * sizeof(char));
	EtiquetteChamp = (char *)malloc(LgMaxChamp * sizeof(char));
	ValeurLue = (char *)malloc(LgMaxVal * sizeof(char));
	if (LigneLue == NULL) {
		printf("Erreur - memoire insuffisante dans le sous programme LireJeuDeParametres \n");
		exit(1);
	}

	NumeroLigne = 1;

	/*Lecture des lignes*/
	while (fgets(LigneLue, LgMaxLigne, Flot) != NULL) {

		/*On retire l'eventuel commentaire en fin de ligne*/
		LigneLue = strtok(LigneLue, "#");
		/*On exclut le cas d'une ligne entiere de commentaire ou de la ligne d'en-tete*/
		if (LigneLue != NULL && strstr(LigneLue, "Champ") == NULL) {
			EtiquetteChamp = strtok(LigneLue, ";");
			ValeurLue = strtok(NULL, ";");
			if (ValeurLue == NULL) {
				printf("Warning - Probleme de lecture du parametre a la ligne %d, seul un champ a ete lu\n",
					NumeroLigne);
			}
			else if (strtok(NULL, ";") != NULL) {
				printf("Warning - Probleme de lecture du parametre a la ligne %d, plus de deux champs ont ete lus\n",
					NumeroLigne);
			}
			else {
				/*On retire le retour a la ligne*/
				ValeurLue = strtok(ValeurLue, "\n");
				PNE_SetParamSimple(Params, EtiquetteChamp, ValeurLue);
			}
		}
	
		NumeroLigne++;
	}

	/* On libere le flot */
	fclose(Flot);

	/* On indique les nouvelles valeurs differentes de l'instance par defaut */
	if (verbose) {
		printf("Differences aux valeurs par defaut : \n");
		printf(PNE_EcrireParams(Params, 1) );
	}
}

void SPX_LireJeuDeParametres_AvecNom(SPX_PARAMS *Params, char const *file_name, int verbose)
{
	FILE * Flot; int LgMaxChamp; int LgMaxVal; int LgMaxLigne;
	char * LigneLue; char * EtiquetteChamp; char * ValeurLue;
	int NumeroLigne;

	LgMaxChamp = 50;
	LgMaxVal = 30;
	LgMaxLigne = LgMaxChamp + LgMaxVal + 1;

	/* Ouverture du fichier en lecture */
	Flot = fopen(file_name, "r");
	if (Flot == NULL) {
		printf("Erreur - l'ouverture du fichier contenant le jeu de parametres a echoue\n");
		exit(1);
	}

	LigneLue = (char *)malloc(LgMaxLigne * sizeof(char));
	EtiquetteChamp = (char *)malloc(LgMaxChamp * sizeof(char));
	ValeurLue = (char *)malloc(LgMaxVal * sizeof(char));
	if (LigneLue == NULL) {
		printf("Erreur - memoire insuffisante dans le sous programme LireJeuDeParametres \n");
		exit(1);
	}

	NumeroLigne = 1;

	/*Lecture des lignes*/
	while (fgets(LigneLue, LgMaxLigne, Flot) != NULL) {

		/*On retire l'eventuel commentaire en fin de ligne*/
		LigneLue = strtok(LigneLue, "#");
		/*On exclut le cas d'une ligne entiere de commentaire ou de la ligne d'en-tete*/
		if (LigneLue != NULL && strstr(LigneLue, "Champ") == NULL) {
			EtiquetteChamp = strtok(LigneLue, ";");
			ValeurLue = strtok(NULL, ";");
			if (ValeurLue == NULL) {
				printf("Warning - Probleme de lecture du parametre a la ligne %d, seul un champ a ete lu\n",
					NumeroLigne);
			}
			else if (strtok(NULL, ";") != NULL) {
				printf("Warning - Probleme de lecture du parametre a la ligne %d, plus de deux champs ont ete lus\n",
					NumeroLigne);
			}
			else {
				/*On retire le retour a la ligne*/
				ValeurLue = strtok(ValeurLue, "\n");
				SPX_SetParamSimple(Params, EtiquetteChamp, ValeurLue);
			}
		}

		NumeroLigne++;
	}

	/* On libere le flot */
	fclose(Flot);

	/* On indique les nouvelles valeurs differentes de l'instance par defaut */
	if (verbose) {
		printf("Differences aux valeurs par defaut : \n");
		printf(SPX_EcrireParams(Params, 1));
	}
}