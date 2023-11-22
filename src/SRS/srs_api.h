// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

#pragma once
#include "sirius_callback.h"
#include "srs_problem.h"
#include "srs_problem_functions.h"
#include <float.h>

//#define SRS_infinite		HUGE_VAL
#define SRS_infinite		1.e+80
#define SRS_CONTINUOUS_VAR	REEL
#define SRS_INTEGER_VAR		ENTIER
#define SRS_GREATER_THAN	'>'
#define SRS_LESSER_THAN		'<'
#define SRS_RANGE			'N'
#define SRS_EQUAL			'='

#define SRS_STATUS_NEW						-1
#define SRS_STATUS_OPTIMAL					 0
#define SRS_STATUS_TIMEOUT_WITH_SOL			 1
#define SRS_STATUS_SOL_FOUND_WITH_UNFEAS	-2
#define SRS_STATUS_NO_SOL_FOUND				-3
#define SRS_STATUS_UNBOUNDED				-4
#define SRS_STATUS_UNFEASIBLE				-5
#define SRS_STATUS_INTERNAL_ERROR			-99

#define SRS_PARAM_VERBOSE_SPX				"VERBOSE_SPX"
#define SRS_PARAM_VERBOSE_PNE				"VERBOSE_PNE"

#define SRS_PARAM_MAXIMIZE					"MAXIMIZE"
#define SRS_PARAM_PRESOLVE					"PRESOLVE"
#define SRS_PARAM_SCALING					"SCALING"

#define SRS_PARAM_RELATIVE_GAP				"RELATIVE_GAP"
#define SRS_PARAM_MAX_TIME					"MAX_TIME"

#define SRS_FORCE_PNE				"FORCE_PNE"
