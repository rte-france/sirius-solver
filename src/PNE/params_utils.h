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
#ifndef PARAMS_UTILS_DEJA_DEFINIES

int EcrireLigneParam_char(char *EtiquetteChamp, char Valeur, char * Ligne);
int EcrireLigneParam_int(char *EtiquetteChamp, int Valeur, char * Ligne);
int EcrireLigneParam_double(char *EtiquetteChamp, double Valeur, char * Ligne);

char ExtraireValeur_char(char *Valeur);
int ExtraireValeur_int(char *Valeur);
double ExtraireValeur_double(char *Valeur);

#define PARAMS_UTILS_DEJA_DEFINIES 1
#endif
# ifdef __cplusplus
  }
# endif	