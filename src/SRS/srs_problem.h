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
} SRS_PROBLEM;

# ifdef __cplusplus
}
# endif
