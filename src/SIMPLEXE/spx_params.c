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
#include "spx_sys.h"
#include "spx_constantes_externes.h"
#include "spx_constantes_internes.h"
#include "spx_params.h"
#include "params_utils.h"
	
SPX_PARAMS * newDefaultSpxParams()
{
	SPX_PARAMS * defaultPneParams = (SPX_PARAMS*) malloc(sizeof(SPX_PARAMS));
#define MACRO_PRM(prm_name, prm_type, prm_default) defaultPneParams->prm_name = prm_default
#define PRM_SEP ;
	SPX_PARAMS_LIST;
	SPX_PARAMS_LIST_FIXED;
#undef PRM_SEP
#undef MACRO_PRM

	return defaultPneParams;
};

# ifdef __cplusplus
  }
# endif		
