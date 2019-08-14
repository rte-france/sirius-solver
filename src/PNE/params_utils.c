// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

#include "params_utils.h"
#include "stdlib.h"
#include "stdio.h"

int EcrireLigneParam_char(char *EtiquetteChamp, char Valeur, char * Ligne)
{
	return sprintf(Ligne, "%s;%d\n", EtiquetteChamp, Valeur);
}

int EcrireLigneParam_int(char *EtiquetteChamp, int Valeur, char * Ligne)
{
	return sprintf(Ligne, "%s;%d\n", EtiquetteChamp, Valeur);
}

int EcrireLigneParam_double(char *EtiquetteChamp, double Valeur, char * Ligne)
{
	return sprintf(Ligne, "%s;%.3e\n", EtiquetteChamp, Valeur);
}

char ExtraireValeur_char(char *Valeur)
{
	return Valeur[0];
}

int ExtraireValeur_int(char *Valeur)
{
	int ExtractedValue; int NbChamps;
	NbChamps = sscanf(Valeur, "%d", &ExtractedValue);
	if (NbChamps != 1) {
		printf("Erreur - la valeur %s est invalide\n", Valeur);
		exit(1);
	}
	return ExtractedValue;
}

double ExtraireValeur_double(char *Valeur)
{
	double ExtractedValue;
	/*int NbChamps;
	NbChamps = sscanf(Valeur, "%e", &ExtractedValue);
	if (NbChamps != 1) {
	printf("Erreur - la valeur %s est invalide\n", Valeur);
	exit(1);
	}
	*/
	ExtractedValue = atof(Valeur);
	if (ExtractedValue == 0.0) printf("Warning - la valeur %s est nulle ou invalide\n", Valeur);
	return ExtractedValue;
}