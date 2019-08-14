// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

#ifndef __MPS_DEFINE__
#define __MPS_DEFINE__

# define OUI_MPS  1
# define NON_MPS  0

# define REEL    1
# define ENTIER  2

# define NON_DEFINI 128

typedef struct PROBLEME_MPS {

int NentreesVar;
int NentreesCnt;

int NbVar;
int NbCnt;

int NbCntRange;

int CoeffHaschCodeContraintes;
int SeuilHaschCodeContraintes;
int CoeffHaschCodeVariables;
int SeuilHaschCodeVariables; 

int   *  Mdeb;
double *  A;      
int   *  Nuvar;  
int   *  Msui;   
int   *  Mder;
int   *  NbTerm;
double *  Rhs;
char   *  SensDeLaContrainte;
double *  BRange; /* Pas nul si contrainte range */
double *  VariablesDualesDesContraintes;

char   ** LabelDeLaContrainte;
char   ** LabelDuSecondMembre;
char   *  LabelDeLObjectif;

char   ** LabelDeLaVariable; 

int   *  TypeDeVariable;  
int   *  TypeDeBorneDeLaVariable;  
double *  U;	     
double *  CoefsObjectif; 	      
double *  Umin;	     
double *  Umax;	

int * FirstNomCnt;
int * NomCntSuivant;

int * FirstNomVar;
int * NomVarSuivant;

} PROBLEME_MPS;

#endif