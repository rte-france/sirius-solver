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

// *** Problem creation/modifications ***
SRS_PROBLEM *	SRScreateprob();
int				SRSfreeprob(SRS_PROBLEM * problem_srs);

int	SRScreatecols(SRS_PROBLEM * problem_srs,
	int nb_cols,
	const double * obj_coefs,
	const int * col_types,
	const double * lb,
	const double * ub,
	char const *const * col_names);
int	SRScreaterows(SRS_PROBLEM * problem_srs,
	int nb_rows,
	const double * rhs,
	const double * range,
	const char * sense,
	char const *const * row_names);
int	SRSsetcoefs(SRS_PROBLEM * problem_srs,
	int   * matBeg, // Pour chaque ligne, indice debut de la ligne dans le vecteur des coefficients
	int   * nbTermsPerRow, // Nombre de termes non nuls de la ligne
	int   * termsColIdx, // Vecteur parallele au suivant. Il contient l'indice colonne de chaque coefficient
	double * coefs); // Coefficients de la matrice des contraintes

int SRSchgbds(SRS_PROBLEM * problem_srs, size_t nbColIndexes, const int * colIndexes, const double * newLb, const double * newUb);
int SRSchgobj(SRS_PROBLEM * problem_srs, size_t nbColIndexes, const int * colIndexes, const double * newObj);
int SRSchgrhs(SRS_PROBLEM * problem_srs, size_t nbRowIndexes, const int * rowIndexes, const double * newRhs);
int SRSchgsens(SRS_PROBLEM * problem_srs, size_t nbRowIndexes, const int * rowIndexes, const char * newSense);
int SRSchgrangeval(SRS_PROBLEM * problem_srs, size_t nbRowIndexes, const int * rowIndexes, const double * newRngVal);
int SRScopyvarboundstype(SRS_PROBLEM * problem_srs, int * varBoundsTypeValues);
int SRSsetxvalue(SRS_PROBLEM * problem_srs, int varIndex, double xValue);

// *** Access problem information ***
const char * SRSversion();
int SRSgetnbcols(SRS_PROBLEM * problem_srs);
int SRSgetnbrows(SRS_PROBLEM * problem_srs);

// *** Problem and Optimization parameters ***
int SRSsetintparams(SRS_PROBLEM * problem_srs, const char * paramId, int paramValue);
int SRSsetdoubleparams(SRS_PROBLEM * problem_srs, const char * paramId, double paramValue);

// *** Optimization ***
int SRSoptimize(SRS_PROBLEM * problem_srs);

// *** Access solution information ***
int SRSgetobjval(SRS_PROBLEM * problem_srs, double * objVal);
int SRSgetspxitercount(SRS_PROBLEM * problem_srs, int * iterCount);
int SRSgetmipitercount(SRS_PROBLEM * problem_srs, int * iterCount);
int SRSgetmipnodecount(SRS_PROBLEM * problem_srs, int * nodeCount);
int SRSgetx(SRS_PROBLEM * problem_srs, double ** xValues);
int SRSgetproblemstatus(SRS_PROBLEM * problem_srs);
int SRSgetreducedcosts(SRS_PROBLEM * problem_srs, double ** reducedCostValues);
int SRSgetdualvalues(SRS_PROBLEM * problem_srs, double ** dualValues); //getpi PROBLEME_SIMPLEXE CoutsMarginauxDesContraintes / PROBLEME_MIP VariablesDualesDesContraintes ?
int SRSgetcolbasisstatus(SRS_PROBLEM * problem_srs, char ** colStatuses);
int SRSgetbestbound(SRS_PROBLEM * problem_srs, double * bestBoundVal);
int SRSgetrowbasisstatus(SRS_PROBLEM * problem_srs, char ** rowStatuses);

// *** Utils ***
int SRSreadmpsprob(SRS_PROBLEM * problem_srs, const char * fileName);
int SRSwritempsprob(PROBLEME_MPS * problem_mps, const char * fileName);
int SRSfreempsprob(PROBLEME_MPS * problem_mps);
int SPXcopy_problem(PROBLEME_MPS * problem_mps, PROBLEME_SIMPLEXE * problem_simplexe);
int SRScopy_from_problem_simplexe(SRS_PROBLEM * problem_srs, PROBLEME_SIMPLEXE * problem_simplexe);

# ifdef __cplusplus
}
# endif
