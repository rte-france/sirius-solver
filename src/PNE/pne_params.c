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
#include "pne_sys.h"
#include "pne_constantes_externes.h"
#include "pne_params.h"
#include "spx_params.h"
#include "params_utils.h"
	
PNE_PARAMS * newDefaultPneParams()
{
	PNE_PARAMS * defaultPneParams = (PNE_PARAMS*) malloc(sizeof(PNE_PARAMS));
#define MACRO_PRM(prm_name, prm_type, prm_default) defaultPneParams->prm_name = prm_default
#define PRM_SEP ;
	PNE_PARAMS_LIST;

	PNE_PARAMS_LIST_FIXED;
#undef PRM_SEP
#undef MACRO_PRM

	defaultPneParams->spx_params = newDefaultSpxParams();

	return defaultPneParams;
};

void PNE_setIntParam(PNE_PARAMS* Params, char const * param_name, int value)
{

#define MACRO_PRM(prm_name, prm_type, prm_default) if ( (strcmp(#prm_type, "int") == 0) && (strcmp(param_name, #prm_name) == 0) ) { Params->prm_name = value; }
#define PRM_SEP else 
	PNE_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM

	else

#define MACRO_PRM(prm_name, prm_type, prm_default) if ( (strcmp(#prm_type, "int") == 0) && (strcmp(param_name, #prm_name) == 0) ) { Params->spx_params->prm_name = value; }
#define PRM_SEP else 
	SPX_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM

	else {
		printf("Parametre %s de type int non repertorie\n", param_name);
	}
}

void PNE_setDoubleParam(PNE_PARAMS* Params, char const * param_name, double value)
{

#define MACRO_PRM(prm_name, prm_type, prm_default) if ( (strcmp(#prm_type, "double") == 0) && (strcmp(param_name, #prm_name) == 0) ) { Params->prm_name = value; }
#define PRM_SEP else 
	PNE_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM

	else

#define MACRO_PRM(prm_name, prm_type, prm_default) if ( (strcmp(#prm_type, "double") == 0) && (strcmp(param_name, #prm_name) == 0) ) { Params->spx_params->prm_name = value; }
#define PRM_SEP else 
		SPX_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM

	else {
	printf("Parametre %s de type double non repertorie\n", param_name);
	}
}

void PNE_setCharParam(PNE_PARAMS* Params, char const * param_name, char value)
{

#define MACRO_PRM(prm_name, prm_type, prm_default) if ( (strcmp(#prm_type, "char") == 0) && (strcmp(param_name, #prm_name) == 0) ) { Params->prm_name = value; }
#define PRM_SEP else 
	PNE_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM

	else

#define MACRO_PRM(prm_name, prm_type, prm_default) if ( (strcmp(#prm_type, "char") == 0) && (strcmp(param_name, #prm_name) == 0) ) { Params->spx_params->prm_name = value; }
#define PRM_SEP else 
		SPX_PARAMS_LIST
#undef PRM_SEP
#undef MACRO_PRM

	else {
	printf("Parametre %s de type char non repertorie\n", param_name);
	}
}

# ifdef __cplusplus
  }
# endif		
