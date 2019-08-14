// Copyright (c) 20xx-2019, RTE (https://www.rte-france.com)
// See AUTHORS.txt
// This Source Code Form is subject to the terms of the Apache License, version 2.0.
// If a copy of the Apache License, version 2.0 was not distributed with this file, you can obtain one at http://www.apache.org/licenses/LICENSE-2.0.
// SPDX-License-Identifier: Apache-2.0
// This file is part of SIRIUS, a linear problem solver, used in the ANTARES Simulator : https://antares-simulator.org/.

#ifndef __LIBRARY_PNE__H_
#define __LIBRARY_PNE__H_

#include "shared_exports.h"
#ifdef __cplusplus
extern "C" {
#endif
	typedef void* PNE_PROB;

	// PNE interface
	void SHARED_EXPORT PNEnewProb(PNE_PROB *);
	void SHARED_EXPORT PNEreadmps(PNE_PROB prob, char const *);

	int SHARED_EXPORT PNEncols(PNE_PROB prob);
	int SHARED_EXPORT PNEnrows(PNE_PROB prob);
	int SHARED_EXPORT PNEneles(PNE_PROB prob);

	int SHARED_EXPORT PNEsolve(PNE_PROB prob);

	// XPRESS interface for PNE problems
	void SHARED_EXPORT launch_xpress(PNE_PROB input);

	void SHARED_EXPORT launch_clp(PNE_PROB input);


	void SHARED_EXPORT launch_spx_clp(PNE_PROB  input);

#ifdef __cplusplus
}
#endif

#endif