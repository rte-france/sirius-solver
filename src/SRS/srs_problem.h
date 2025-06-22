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
#pragma once

#include "mps_define.h"
#include "pne_definition_arguments.h"
#include "spx_definition_arguments.h"
#include "spx_define.h"
#include <stdbool.h>

typedef struct SRS_PROBLEM {
	PROBLEME_MPS * problem_mps;
	PROBLEME_A_RESOUDRE * problem_mip;
	PROBLEME_SIMPLEXE * problem_simplexe;
	PROBLEME_SPX * problem_spx;

	bool read_an_mps;
	bool is_mip;

	// params
	int verboseSpx;
	int verbosePne;
	bool maximize;
	double relativeGap;
	int presolve;
	int scaling;
	double maxTime;
	void *callback;
} SRS_PROBLEM;

# ifdef __cplusplus
}
# endif
